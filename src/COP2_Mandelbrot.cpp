/*
	Cole Clark's Fractal Suite

	COP2_Mandelbrot.cpp
	Code for CC Mandelbrot Generator Cop Node.
 */

// Houdini never finds this dso without this included.
#include <UT/UT_DSOVersion.h>

#include <UT/UT_Matrix3.h>

// For PRMoneDefaults
#include <PRM/PRM_Include.h>
#include <PRM/PRM_ChoiceList.h>

// For FOR_EACH_UNCOOKED_TILE
#include <TIL/TIL_Tile.h>

#include "FractalSpace.h"
#include "COP2_Mandelbrot.h"

using namespace CC;

/// Parm Switcher used by this interface
COP_GENERATOR_SWITCHER(11, "Fractal");

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
static PRM_Name nameRotate("rotate", "Rotate");
static PRM_Name nameXOrd("xOrd", "Xform Order");
static PRM_Name nameIter("iter", "Iterations");
static PRM_Name namePow("pow", "Exponent");
static PRM_Name nameBailout("bailout", "Bailout");
static PRM_Name nameJDepth("jdepth", "Julia Depth");
static PRM_Name nameJOffset("joffset", "Julia Offset");
static PRM_Name nameSep1("sep1", "sep1");
static PRM_Name nameSep2("sep2", "sep2");


/// ChoiceList Lists
static PRM_Name xordMenuNames[] =
{
	PRM_Name("TRS", "Translate Rotate Scale"),
	PRM_Name("TSR", "Translate Scale Rotate"),
	PRM_Name("RTS", "Rotate Translate Scale"),
	PRM_Name("RST", "Rotate Scale Translate"),
	PRM_Name("STR", "Scale Translate Rotate"),
	PRM_Name("SRT", "Scale Rotate Translate"),
	PRM_Name(0)
};

static PRM_ChoiceList xOrdMenu
(
	(PRM_ChoiceListType)(PRM_CHOICELIST_EXCLUSIVE | PRM_CHOICELIST_REPLACE),
	::xordMenuNames
);

/// Declare Parm Defaults
static PRM_Default defaultScale{ 1 };
static PRM_Default defaultIter{ 50 };
static PRM_Default defaultPow{ 2 };
static PRM_Default defaultBailout{ 2 };
static PRM_Default defaultXOrd{ 4 };  // Scale Translate Rotate

/// Deflare Parm Ranges
static PRM_Range rangeScale
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, 20
};

static PRM_Range rangeRotate
{
	PRM_RangeFlag::PRM_RANGE_UI, -180,
	PRM_RangeFlag::PRM_RANGE_UI, 180
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
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameXOrd, &defaultXOrd, &xOrdMenu),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameScale, &defaultScale, 0, &rangeScale),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameOffset, PRMzeroDefaults),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameRotate, PRMzeroDefaults, 0, &rangeRotate),
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSep1, PRMzeroDefaults),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameIter, &defaultIter, 0, &rangeIter),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &namePow, &defaultPow, 0, &rangePow),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameBailout, &defaultBailout, 0, &rangeBailout),
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSep2, PRMzeroDefaults),
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
	float t,           // Not actually sure, maybe tile?
	int,               // xsize
	int,               // ysize
	int,               // thread
	int                // max_num_threads
)
{
	// Create new empty data object.
	COP2_MandelbrotData* data{ new COP2_MandelbrotData };

	// Space Xform Attributes
	float scale = evalFloat(nameScale.getToken(), 0, t);
	const float offset_x = evalFloat(nameOffset.getToken(), 0, t);
	const float offset_y = evalFloat(nameOffset.getToken(), 1, t);
	const double rotate = evalFloat(nameRotate.getToken(), 0, t);

	const RSTORDER xOrd = get_rst_order(evalInt(nameXOrd.getToken(), 0, t));

	// This edit occurs because it's more sensible in the UI the think of the scale
	// as 'Scaling Up' even though in reality we are 'scaling down' into the fractal.
	// It's my opinion that big numbers are easier to work with for artists than
	// infinitesimal numbers. Most fractals are more about zooming deep into them than
	// scaling as far away from them as possible. :)
	scale = 1 / scale;

	data->space.set_xform(
		offset_x,
		offset_y,
		rotate,
		scale,
		scale,
		0.5,  // Center X of the Image
		0.5,  // Center Y of the image
		xOrd);

	// Fractal Attributes
	int iter = evalInt(nameIter.getToken(), 0, t);
	float pow = evalFloat(namePow.getToken(), 0, t);
	float bailout = evalFloat(nameBailout.getToken(), 0, t);
	int jdepth = evalInt(nameJDepth.getToken(), 0, t);
	float joffset_x = evalFloat(nameJOffset.getToken(), 0, t);
	float joffset_y = evalFloat(nameJOffset.getToken(), 1, t);

	data->fractal = Mandelbrot(iter, pow, bailout, jdepth, joffset_x, joffset_y);

	return data;
}

/// Creates the image. This is called by multiple worker threads by Houdini.
OP_ERROR
COP2_Mandelbrot::generateTile(COP2_Context& context, TIL_TileList* tileList)
{
	COP2_MandelbrotData* data{ static_cast<COP2_MandelbrotData*>(context.data()) };

	// Set the size of the fractal space relative to this context's size.
	data->space.set_image_size(context.myXsize, context.myYsize);

	// Initialize float array the size of current tile list for values to write to.
	float *dest = new float[tileList->mySize]{ 0 };

	TIL_Tile* tile;
	int tileIndex;

	// Forward declaring values
	int size_x, size_y;
	exint i;  // Huge because number of pixels may be crazy
	WORLDPIXELCOORDS worldPixel;
	FCOORDS fractalCoords;
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
			{
				// Calculate the 'world pixel', or literally where the pixel is in
				// Terms of screen space, and not tile space.
				worldPixel = CC::calculate_world_pixel(tileList, tile, i);
				
				// Cast this to the 'fractal coords'. This is initialized from the size
				// of the picture plane, with an xform applied from the interface.
				fractalCoords = data->space.get_fractal_coords(worldPixel);

				// Calculate the fractal based on the fractal coords.				
				dest[i] = data->fractal.calculate(fractalCoords);

			}
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

/// Destructor
COP2_MandelbrotData::~COP2_MandelbrotData() {}
