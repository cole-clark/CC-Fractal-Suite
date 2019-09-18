/*
	Cole Clark's Fractal Suite

	COP2_Mandelbrot.cpp
	Code for CC Mandelbrot Generator Cop Node.
 */

#include <CH/CH_Manager.h>
#include <PRM/PRM_ChoiceList.h>

#include "COP2_Mandelbrot.h"

using namespace CC;

/// Parm Switcher used by this interface
COP_GENERATOR_SWITCHER(15, "Fractal");

/// Private Constructor
COP2_Mandelbrot::COP2_Mandelbrot(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry) : COP2_Generator(parent, name, entry) {}

/// Node Specific Parm Information

// Parm Name
static PRM_Name nameMode("mode", "Mode");
static PRM_Name nameFit("fit", "Fit");

// ChoiceList Lists
static PRM_Name modeMenuNames[] =
{
	PRM_Name("smooth", "Smooth"),
	PRM_Name("raw", "Raw"),
	PRM_Name(0)
};

static PRM_ChoiceList modeMenu
(
	(PRM_ChoiceListType)(PRM_CHOICELIST_EXCLUSIVE | PRM_CHOICELIST_REPLACE),
	::modeMenuNames
);

// Declare Parm Defaults
static PRM_Default defaultModeMenu{ 0 };
static PRM_Default defaultFit{ 1 };

/// Create Template List
PRM_Template
COP2_Mandelbrot::myTemplateList[]
{
	TEMPLATE_SWITCHER,
	TEMPLATES_XFORM_MULTI,
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepA),
	TEMPLATES_MANDELBROT,
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepB),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameMode, &defaultModeMenu, &modeMenu),
	PRM_Template(PRM_TOGGLE_J, TOOL_PARM, 1, &nameFit, &defaultFit),
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
	int image_sizex,   // xsize
	int image_sizey,   // ysize
	int,               // thread
	int                // max_num_threads
)
{
	// Create new empty data object.
	COP2_MandelbrotData* data{ new COP2_MandelbrotData };

	// Connect image size.
	data->space.set_image_size(image_sizex, image_sizey);

	// Stash xform data.
	MultiXformStashData multiXformData;
	multiXformData.evalArgs(this, t);
	data->space.set_xform(multiXformData);

	// Stash mandelbrot Data
	MandelbrotStashData mandelData;
	mandelData.evalArgs(this, t);
	data->fractal = Mandelbrot(mandelData);

	// Node-specific parms
	data->mode = static_cast<MandelbrotMode>(
		evalInt(nameMode.getToken(), 0, t));

	data->fit = evalInt(nameFit.getToken(), 0, t);

	return data;
}

/// Creates the image. This is called by multiple worker threads by Houdini.
OP_ERROR
COP2_Mandelbrot::generateTile(COP2_Context& context, TIL_TileList* tileList)
{
	COP2_MandelbrotData* data{static_cast<COP2_MandelbrotData*>(context.data()) };

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
			if (tileIndex == 0)
			{
				WORLDPIXELCOORDS worldPixel = CC::calculate_world_pixel(tileList, tile, i);
				COMPLEX fractalCoords = data->space.get_fractal_coords(worldPixel);
				FractalCoordsInfo pixelInfo = data->fractal.calculate(fractalCoords);

				float val = pixelInfo.smooth;

				if (data->mode == MandelbrotMode::RAW)
					val = pixelInfo.num_iter;

				if (data->fit)
					val /= (float)data->fractal.data.iters;
					
				dest[i] = val;
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
