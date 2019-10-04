/** \file COP2_Pickover.cpp
	Header declaring the Pickover Cop2 Operator.
 */

 // Local
#include "COP2_Pickover.h"

// HDK
#include <CH/CH_Manager.h>

/** Parm Switcher used by this interface to generate default generator parms */
COP_GENERATOR_SWITCHER(15, "Fractal");


CC::COP2_Pickover::COP2_Pickover(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry) : COP2_Generator(parent, name, entry) {}

// Create Template List
PRM_Template
CC::COP2_Pickover::myTemplateList[]
{
	TEMPLATE_SWITCHER,
	TEMPLATES_XFORM_MULTI,
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepA),
	TEMPLATES_MANDELBROT,
	TEMPLATES_PICKOVER,
	PRM_Template()
};

// Assign Template Pair of node to generator
OP_TemplatePair
CC::COP2_Pickover::myTemplatePair
(
	COP2_Pickover::myTemplateList,
	&COP2_Generator::myTemplatePair
);

// Assign empty variable pairing
OP_VariablePair
CC::COP2_Pickover::myVariablePair
(
	0,
	&COP2_Node::myVariablePair
);

// Gets Sequence information.
TIL_Sequence*
CC::COP2_Pickover::cookSequenceInfo(OP_ERROR& error)
{
	COP2_Generator::cookSequenceInfo(error);
	return &mySequence;
}

COP2_ContextData*
CC::COP2_Pickover::newContextData

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
	COP2_PickoverData* data{ new COP2_PickoverData };

	// Set the image size and stash the parameters.
	data->space.set_image_size(image_sizex, image_sizey);

	// Stash xform data.
	MultiXformStashData multiXformData;
	multiXformData.evalArgs(this, t);
	data->space.set_xform(multiXformData);

	// Stash Pickover fractal data
	PickoverStashData pickoverData;
	pickoverData.evalArgs(this, t);
	data->fractal = Pickover(pickoverData);

	// Set the world point for the pickover
	data->world_point = data->space.get_pixel_coords(
		data->fractal.data.popoint);

	return data;
}

// Creates the image. This is called by multiple worker threads by Houdini.
OP_ERROR
CC::COP2_Pickover::generateTile(
	COP2_Context& context,
	TIL_TileList* tileList)
{
	COP2_PickoverData* data{ static_cast<COP2_PickoverData*>(context.data()) };

	// Initialize float array the size of current
	// tile list for values to write to.
	// Set the default value to black.
	fpreal32 *dest = new fpreal32[tileList->mySize]{ 0 };

	TIL_Tile* tile;
	int tileIndex;

	// Forward declaring values
	int size_x, size_y;
	exint i;  // Huge because number of pixels may be crazy
	WORLDPIXELCOORDS worldPixel;
	COMPLEX fractalCoords;

	// For each pixel in tile...
	FOR_EACH_UNCOOKED_TILE(tileList, tile, tileIndex)
	{
		tile->getSize(size_x, size_y);
		// Cook the fractal for the first channel always, and the second
		// 'reference' channel if data.poref is on.
		// But don't cook the fractal in unnecessary image planes

		for (exint i = 0; i < size_x * size_y; i++)
		{
			//First check to see if this frame should be cooked
			// If index is greater than one, or is one but reference isn't on
			// Assign black
			if (tileIndex > 1 || (tileIndex == 1 && !data->fractal.data.poref))
				dest[i] = 0.0f;
			else
			{
				// Get the 'world pixel coords from tile.
				WORLDPIXELCOORDS worldPixel = CC::calculate_world_pixel(
					tileList, tile, i);
				// Convert those to 'fractal space'
				COMPLEX fractalCoords = data->space.get_fractal_coords(
					worldPixel);
				FractalCoordsInfo pixelInfo = data->fractal.calculate(
					fractalCoords);

				// Write the main pickover fractal
				if (tileIndex == 0)  // Main Fractal
				{
					dest[i] = pixelInfo.smooth;
				}
				// Write the reference fractal
				else if (tileIndex == 1 && data->fractal.data.poref)
				{
					dest[i] = data->calculate_reference(
						fractalCoords,
						worldPixel);
				}
			}
		}
;

		writeFPtoTile(tileList, dest, tileIndex);
	};

	delete[] dest;

	return error();
}

bool
CC::COP2_Pickover::updateParmsFlags()
{
	// Determine Mode State
	fpreal t = CHgetEvalTime();
	bool modeRotate = evalInt(namePoMode.getToken(), 0, t);
	bool modePoRef = evalInt(POREFERENCE_NAME.first, 0, t);

	// Set variables for hiding
	bool displayRotate{ false };
	bool displayPoRefSize{ false };

	if (modeRotate)
		displayRotate = true;

	if (modePoRef)
		displayPoRefSize = true;

	// Call parent's updateParmFlags to avoid recursion.
	bool changed = COP2_Generator::updateParmsFlags();

	changed |= setVisibleState(namePoLineRotate.getToken(), displayRotate);
	changed |= setVisibleState(POREFSIZE_NAME.first, displayPoRefSize);

	return changed;
}

// Destructor
CC::COP2_Pickover::~COP2_Pickover() {}

fpreal32
CC::COP2_PickoverData::calculate_reference(
	COMPLEX fractalCoords, WORLDPIXELCOORDS worldPixel)
{
	// Acts as a typedef
	fpreal& refsize = fractal.data.porefsize;

	// Get distance of the 'world point' to the current pixel
	fpreal distance = SYSsqrt(
		SYSpow(world_point.first - worldPixel.first, 2.0) +
		SYSpow(world_point.second - worldPixel.second, 2.0));

	// Casted down from double to single precision because we are returning
	// FP values intended for an image, and the COPs maximum FP depth is 32 bit
	fpreal32 val = 0.0;

	// Set val to the 0-1 distance, if pomode in point mode
	if (distance < refsize && !fractal.data.pomode)
		val = (refsize - distance) / refsize;

	if (fractal.data.pomode)  // If in line mode
	{
		distance = fractal.distance_to_line(
			fractalCoords,
			fractal.data.popoint,
			fractal.data.porotate);

		// Set 0-1 size if within the distance range.
		// This 1000x smaller multiplier is fudging the difference between
		// A 'fractal space' distance and a 'world pixel' space distance.
		// This is acceptable for a reference line,
		// though not mathematically precise. Didn't seem worth the CPU cycles
		// to me to calculate accurately.
		if (distance < refsize * 0.001)
			val = (refsize - distance) * 1000;
		else
			val = 0.0f;  // Black
	}

	return val;
}

// Destructor
CC::COP2_PickoverData::~COP2_PickoverData() {}