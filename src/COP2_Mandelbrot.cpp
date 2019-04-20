/*
	Cole Clark's Fractal Suite

	COP2_Mandelbrot.cpp
	Code for CC Mandelbrot Generator Cop Node.
 */

// Houdini never finds this dso without this included.
#include <UT/UT_DSOVersion.h>

// For PRMoneDefaults
#include <PRM/PRM_Include.h>

// For FOR_EACH_UNCOOKED_TILE
#include <TIL/TIL_Tile.h>

#include "COP2_Mandelbrot.h"

using namespace CC;

/// Parm Switcher used by this interface
COP_GENERATOR_SWITCHER(7, "Fractal");

/// Private Constructor
COP2_Mandelbrot::COP2_Mandelbrot(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry) : COP2_Generator(parent, name, entry) {}

/// Public Constructor
OP_Node *
COP2_Mandelbrot::myConstructor(
	OP_Network* net,
	const char* name,
	OP_Operator* op)
{
	return new COP2_Mandelbrot(net, name, op);
}

/// Declare Parm Names
static PRM_Name nameScale("scale", "Scale");
static PRM_Name nameOffset("offset", "Offset");
static PRM_Name nameIter("iter", "Iterations");
static PRM_Name namePow("pow", "Exponent");
static PRM_Name nameBailout("bailout", "Bailout");
static PRM_Name nameJDepth("jdepth", "Julia Depth");
static PRM_Name nameJOffset("joffset", "Julia Offset");

/// Declare Parm Defaults
static PRM_Default defaultScale{ 1 };
static PRM_Default defaultIter{ 50 };
static PRM_Default defaultPow{ 2 };
static PRM_Default defaultBailout{ 2 };

/// Deflare Parm Ranges
static PRM_Range rangeScale
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, 20
};

static PRM_Range rangeIter
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 1,
	PRM_RangeFlag::PRM_RANGE_UI, 200
};


static PRM_Range rangePow
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, 10
};

static PRM_Range rangeBailout
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, 4
};

static PRM_Range rangeJDepth
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, 5
};

/// Create Template List
PRM_Template
COP2_Mandelbrot::myTemplateList[]
{
    // The Cop2 generator defaults to having 3 tabs: Mask, Image, Sequence. +1 for ours.
	PRM_Template(PRM_SWITCHER, 4, &PRMswitcherName, switcher),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameScale, &defaultScale, 0, &rangeScale),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameOffset, PRMzeroDefaults),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameIter, &defaultIter, 0, &rangeIter),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &namePow, &defaultPow, 0, &rangePow),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameBailout, &defaultBailout, 0, &rangeBailout),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameJDepth, PRMzeroDefaults, 0, &rangeJDepth),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameJOffset, PRMzeroDefaults),
	PRM_Template()
};

/// Assign Template Pair of node to generator
OP_TemplatePair COP2_Mandelbrot::myTemplatePair
(
	COP2_Mandelbrot::myTemplateList,
	&COP2_Generator::myTemplatePair
);

/// Assign empty variable pairing
OP_VariablePair COP2_Mandelbrot::myVariablePair
(
	0,
	&COP2_Node::myVariablePair
);

/// Gets Sequence information.
TIL_Sequence*
COP2_Mandelbrot::cookSequenceInfo(OP_ERROR& error)
{
	COP2_Generator::cookSequenceInfo(error);

	return &mySequence;
}

/// Stashes data for cooking image
COP2_ContextData*
COP2_Mandelbrot::newContextData

(
	const TIL_Plane*,  // planename
	int,               // array index
	float t,           // Not actually sure, maybe tile
	int,               // xsize
	int,               // ysize
	int,               // thread
	int                // max_num_threads
)
{
	COP2_MandelbrotData* data{ new COP2_MandelbrotData };

	/// Stash values by cooking the node
	data->scale = evalFloat(nameScale.getToken(), 0, t);
	data->offset.x() = evalFloat(nameOffset.getToken(), 0, t);
	data->offset.y() = evalFloat(nameOffset.getToken(), 1, t);
	data->iter = evalInt(nameIter.getToken(), 0, t);
	data->pow = evalFloat(namePow.getToken(), 0, t);
	data->bailout = evalFloat(nameBailout.getToken(), 0, t);
	data->jdepth = evalInt(nameJDepth.getToken(), 0, t);
	data->joffset.x() = evalFloat(nameJOffset.getToken(), 0, t);
	data->joffset.y() = evalFloat(nameJOffset.getToken(), 1, t);

	return data;
}

/// Creates the image. This is called by multiple worker threads by Houdini.
OP_ERROR
COP2_Mandelbrot::generateTile(COP2_Context& context, TIL_TileList* tileList)
{
	COP2_MandelbrotData* data{ static_cast<COP2_MandelbrotData*>(context.data()) };

	// Initialize float array the size of current tile list for values to write to.
	float *dest = new float[tileList->mySize]{ 0 };

	TIL_Tile* tile;
	int tileIndex;

	// Forward declaring values
	int size_x, size_y;
	exint i;  // Huge because number of pixels may be crazy

	// Comes from TIL/TIL_Tile.h
	FOR_EACH_UNCOOKED_TILE(tileList, tile, tileIndex)
	{
		tile->getSize(size_x, size_y);

		for (i = 0; i < size_x * size_y; i++)
		{
			// Check the tile index, and black out if not first.
			// This is done to keep user from cooking redundant fractals, but
			// They can choose which image plane the fractal goes onto by making it
			// The first.
			if (tileIndex == 0)
				dest[i] = 1.0f;
			else
				dest[i] = 0.0f;
		}

		writeFPtoTile(tileList, dest, tileIndex);
	};

	delete[] dest;

	return error();
}

/// Destructor
COP2_Mandelbrot::~COP2_Mandelbrot() {}

/// Declare default values when constructing Data Object
COP2_MandelbrotData::COP2_MandelbrotData()
{
	scale = defaultScale.getFloat();
	offset = (0.0f, 0.0f);
	iter = defaultIter.getOrdinal();
	pow = defaultPow.getFloat();
	bailout = defaultBailout.getFloat();
	jdepth = 0;
	joffset = (0.0f, 0.0f);
}

/// Destructor
COP2_MandelbrotData::~COP2_MandelbrotData() {}
