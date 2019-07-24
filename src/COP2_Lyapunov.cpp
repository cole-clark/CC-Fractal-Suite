/*
	Cole Clark's Fractal Suite

	COP2_Lyapunov.cpp
	Code for CC Lyapunov Fracatal Generator Cop Node.
 */ 

#include "COP2_Lyapunov.h"
#include "FractalNode.h"

COP_GENERATOR_SWITCHER(10, "Fractal");

using namespace CC;

/// Declare Parm Names
static PRM_Name nameSeq("seq", "Sequence");
static PRM_Name nameSeqWeights("weight_#", "Weight #");
static PRM_Name nameStart("seqstart", "Seq Start Value");
static PRM_Name nameMaxValue("maxval", "Max Value");
static PRM_Name nameMinMax("minmax", "Min Max");

/// Declare Defaults
static PRM_Default defaultMaxValue(8192);
static PRM_Default defaultStart(0.5);
static PRM_Default defaultMinMax[] = { 0.0f, 1.0f };

/// Deflare Parm Ranges
static PRM_Range rangeMaxValue
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 1,
	PRM_RangeFlag::PRM_RANGE_UI, defaultMaxValue.getFloat()
};

/// Private Constructor
COP2_Lyapunov::COP2_Lyapunov(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry) : COP2_Generator(parent, name, entry) {}

TIL_Sequence* COP2_Lyapunov::cookSequenceInfo(OP_ERROR & error)
{
	COP2_Generator::cookSequenceInfo(error);

	return &mySequence;
}

/// Stashes data for cooking image
COP2_ContextData*
COP2_Lyapunov::newContextData

(
	const TIL_Plane*,  // planename
	int,               // array index
	float t,
	int image_sizex,   // xsize
	int image_sizey,   // ysize
	int,               // thread
	int                // max_num_threads
)
{
	COP2_LyapunovData* data{ new COP2_LyapunovData };
	// Set the image size and stash the parameters.
	data->space.set_image_size(image_sizex, image_sizey);

	XformStashData xformData;
	xformData.evalArgs(this, t);
	data->space.set_xform(xformData);

	LyapunovStashData lyaData;
	lyaData.evalArgs(this, t);
	data->fractal = Lyapunov(lyaData);

	return data;
}

/// Assign Template Pair of node to generator
OP_TemplatePair COP2_Lyapunov::myTemplatePair
(
	COP2_Lyapunov::myTemplateList,
	&COP2_Generator::myTemplatePair
);

/// Assign empty variable pairing
OP_VariablePair COP2_Lyapunov::myVariablePair
(
	0,
	&COP2_Node::myVariablePair
);

/// Multiparm Template
static PRM_Template multiparmSeqTemps[] =
{
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameSeqWeights, PRMzeroDefaults),
	PRM_Template()
};

/// Create Template List
PRM_Template
COP2_Lyapunov::myTemplateList[]
{
	TEMPLATE_SWITCHER,
	TEMPLATES_XFORM,
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepA),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameIter, &defaultIter, 0, &rangeIter),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameStart, &defaultStart),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameMaxValue, &defaultMaxValue, 0, &rangeMaxValue),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameMinMax, defaultMinMax),
	PRM_Template(PRM_MULTITYPE_LIST, multiparmSeqTemps, 1, &nameSeq,PRMoneDefaults),
	PRM_Template()
};

/// Creates the image. This is called by multiple worker threads by Houdini.
OP_ERROR
COP2_Lyapunov::generateTile(COP2_Context& context, TIL_TileList* tileList)
{
	COP2_LyapunovData* data{ static_cast<COP2_LyapunovData*>(context.data()) };

	// Initialize float array the size of current tile list for values to write to.
	float *dest = new float[tileList->mySize]{ 0 };

	TIL_Tile* tile;
	int tileIndex;

	// Forward declaring values
	int size_x, size_y;
	exint i;  // Huge because number of pixels may be crazy
	WORLDPIXELCOORDS worldPixel;
	COMPLEX fractalCoords;
	// Comes from TIL/TIL_Tile.h
	FOR_EACH_UNCOOKED_TILE(tileList, tile, tileIndex)
	{
		tile->getSize(size_x, size_y);

		for (exint i = 0; i < size_x * size_y; i++)
		{
			WORLDPIXELCOORDS worldPixel = CC::calculate_world_pixel(tileList, tile, i);
			COMPLEX fractalCoords = data->space.get_fractal_coords(worldPixel);
			FractalCoordsInfo pixelInfo = data->fractal.calculate(fractalCoords);

			if (tileIndex == 0)
			{
				dest[i] = pixelInfo.num_iter;
			}
			else
				dest[i] = 0.0f;
		}

		writeFPtoTile(tileList, dest, tileIndex);
	};

	delete[] dest;

	return error();
}

CC::COP2_LyapunovData::~COP2_LyapunovData()
{
}
