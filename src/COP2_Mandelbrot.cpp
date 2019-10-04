/** \file COP2_Mandelbrot.cpp
	Source declaring the Lyapunov Cop2 Operator.
 */

 // Local
#include "COP2_Mandelbrot.h"

// HDK
#include <CH/CH_Manager.h>
#include <PRM/PRM_ChoiceList.h>

/** Parm Switcher used by this interface to generate default generator parms */
COP_GENERATOR_SWITCHER(12, "Fractal");


CC::COP2_Mandelbrot::COP2_Mandelbrot(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry) : COP2_Generator(parent, name, entry) {}

// Node Specific Parm Information
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


PRM_Template
CC::COP2_Mandelbrot::myTemplateList[]
{
	TEMPLATE_SWITCHER,
	TEMPLATES_XFORM_MULTI,
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepA),
	TEMPLATES_MANDELBROT,
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepB),
	PRM_Template(
		PRM_INT_J, TOOL_PARM, 1, &nameMode, &defaultModeMenu, &modeMenu),
	PRM_Template(PRM_TOGGLE_J, TOOL_PARM, 1, &nameFit, &defaultFit),
	PRM_Template()
};


OP_TemplatePair
CC::COP2_Mandelbrot::myTemplatePair
(
	COP2_Mandelbrot::myTemplateList,
	&COP2_Generator::myTemplatePair
);

// Assign empty variable pairing
OP_VariablePair
CC::COP2_Mandelbrot::myVariablePair
(
	0,
	&COP2_Node::myVariablePair
);

// Gets Sequence information.
TIL_Sequence*
CC::COP2_Mandelbrot::cookSequenceInfo(OP_ERROR& error)
{
	COP2_Generator::cookSequenceInfo(error);

	return &mySequence;
}


COP2_ContextData*
CC::COP2_Mandelbrot::newContextData

(
	const TIL_Plane* planename, /** planename */
	int index,                  /** array index */
	fpreal32 t,                 /** time*/
	int image_sizex,            /** xsize*/
	int image_sizey,            /** ysize*/
	int thread,                 /** thread*/
	int maxthreads              /** max_num_threads*/
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

// Creates the image. This is called by multiple worker threads by Houdini.
OP_ERROR
CC::COP2_Mandelbrot::generateTile(
	COP2_Context& context,
	TIL_TileList* tileList)
{
	COP2_MandelbrotData* data{
		static_cast<COP2_MandelbrotData*>(context.data()) };

	// Initialize float array the size of current tile list to write values to
	fpreal32 *dest = new fpreal32[tileList->mySize]{ 0 };

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

		// For each pixel in tile...
		for (exint i = 0; i < size_x * size_y; i++)
		{
			// Only calculate the fractal for the first Red Channel
			if (tileIndex == 0)
			{
				// Get the 'world pixel coords from tile.
				WORLDPIXELCOORDS worldPixel = CC::calculate_world_pixel(
					tileList,
					tile,
					i);

				// Convert those to 'fractal space'
				COMPLEX fractalCoords = data->space.get_fractal_coords(
					worldPixel);

				// Calculate the fratal from the new fractal coordinates
				FractalCoordsInfo pixelInfo = data->fractal.calculate(
					fractalCoords);

				// Determine whether to return smooth or raw values
				fpreal32 val = pixelInfo.smooth;

				if (data->mode == MandelbrotMode::RAW)
					val = pixelInfo.num_iter;

				// Optionally normalize the values
				if (data->fit)
					val /= (fpreal64)data->fractal.data.iters;

				// Assign value to the pixel
				dest[i] = (fpreal32)val;
			}
			else // Other image planes, black.
				dest[i] = 0.0f;
		}

		writeFPtoTile(tileList, dest, tileIndex);
	};

	delete[] dest;

	return error();
}

/// Destructor
CC::COP2_Mandelbrot::~COP2_Mandelbrot() {}

/// Destructor
CC::COP2_MandelbrotData::~COP2_MandelbrotData() {}
