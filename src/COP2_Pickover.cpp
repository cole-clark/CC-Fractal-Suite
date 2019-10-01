/*
	Cole Clark's Fractal Suite

	COP2_Pickover.cpp
	Code for CC Pickover Fracatal Generator Cop Node.
 */

#include "COP2_Pickover.h"
#include <CH/CH_Manager.h>

using namespace CC;

/// Parm Switcher used by this interface
COP_GENERATOR_SWITCHER(15, "Fractal");

 /// Private Constructor
COP2_Pickover::COP2_Pickover(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry) : COP2_Generator(parent, name, entry) {}

/// Create Template List
PRM_Template
COP2_Pickover::myTemplateList[]
{
	TEMPLATE_SWITCHER,
	TEMPLATES_XFORM_MULTI,
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepA),
	TEMPLATES_MANDELBROT,
	TEMPLATES_PICKOVER,
	PRM_Template()
};

/// Assign Template Pair of node to generator
OP_TemplatePair COP2_Pickover::myTemplatePair
(
	COP2_Pickover::myTemplateList,
	&COP2_Generator::myTemplatePair
);

/// Assign empty variable pairing
OP_VariablePair COP2_Pickover::myVariablePair
(
	0,
	&COP2_Node::myVariablePair
);

/// Gets Sequence information.
TIL_Sequence*
COP2_Pickover::cookSequenceInfo(OP_ERROR& error)
{
	COP2_Generator::cookSequenceInfo(error);

	return &mySequence;
}

/// Stashes data for cooking image
COP2_ContextData*
COP2_Pickover::newContextData

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
	// Create new empty data object.
	COP2_PickoverData* data{ new COP2_PickoverData };
	
	// Set the image size and stash the parameters.
	data->space.set_image_size(image_sizex, image_sizey);
	
	// Stash xform data.
	MultiXformStashData multiXformData;
	multiXformData.evalArgs(this, t);
	data->space.set_xform(multiXformData);

	PickoverStashData pickoverData;
	pickoverData.evalArgs(this, t);
	data->fractal = Pickover(pickoverData);

	// Set the world point for the pickover
	data->world_point = data->space.get_pixel_coords(data->fractal.data.popoint);

	return data;
}

/// Creates the image. This is called by multiple worker threads by Houdini.
OP_ERROR
COP2_Pickover::generateTile(COP2_Context& context, TIL_TileList* tileList)
{
	COP2_PickoverData* data{ static_cast<COP2_PickoverData*>(context.data()) };

	// Initialize float array the size of current tile list for values to write to.
	float *dest = new float[tileList->mySize]{ 0 };

	TIL_Tile* tile;
	int tileIndex;

	// Forward declaring values
	int size_x, size_y;
	exint i;  // Huge because number of pixels may be crazy
	WORLDPIXELCOORDS worldPixel;
	COMPLEX fractalCoords;

	FOR_EACH_UNCOOKED_TILE(tileList, tile, tileIndex)
	{
		tile->getSize(size_x, size_y);

		for (exint i = 0; i < size_x * size_y; i++)
		{
			WORLDPIXELCOORDS worldPixel = CC::calculate_world_pixel(tileList, tile, i);
			COMPLEX fractalCoords = data->space.get_fractal_coords(worldPixel);
			FractalCoordsInfo pixelInfo = data->fractal.calculate(fractalCoords);

			if (tileIndex == 0)  // Main Fractal
			{
				dest[i] = pixelInfo.smooth;
			}
			
			else if (tileIndex == 1 && data->fractal.data.poref)  // Reference image
			{
				dest[i] = data->calculate_reference(fractalCoords, worldPixel);
			}

			else
				dest[i] = 0.0f;
		}

		writeFPtoTile(tileList, dest, tileIndex);
	};

	delete[] dest;

	return error();
}

bool COP2_Pickover::updateParmsFlags()
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

/// Destructor
COP2_Pickover::~COP2_Pickover() {}

float COP2_PickoverData::calculate_reference(
	COMPLEX fractalCoords, WORLDPIXELCOORDS worldPixel)
{
	// Acts as a typedef
	double& refsize = fractal.data.porefsize;

	// Get distance of the 'world point' to the current pixel
	double distance = SYSsqrt(
		SYSpow(world_point.first - worldPixel.first, 2.0) +
		SYSpow(world_point.second - worldPixel.second, 2.0));

	float val = 0.0;

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
		// This is acceptable for a reference line, but not an exact comparison.
		if (distance < refsize * 0.001)
			val = (refsize - distance) * 1000;
		else
			val = 0.0f;
	}

	return val;
}

/// Destructor
COP2_PickoverData::~COP2_PickoverData() {}