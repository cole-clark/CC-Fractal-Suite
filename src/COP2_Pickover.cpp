/*
	Cole Clark's Fractal Suite

	COP2_Pickover.cpp
	Code for CC Pickover Fracatal Generator Cop Node.
 */

#include "COP2_Pickover.h"
#include <CH/CH_Manager.h>

using namespace CC;

/// Parm Switcher used by this interface
COP_GENERATOR_SWITCHER(16, "Fractal");

 /// Private Constructor
COP2_Pickover::COP2_Pickover(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry) : COP2_Generator(parent, name, entry) {}

/// Define node-specific attributes
static PRM_Name namePoPoint(POPOINT_NAME.first, POPOINT_NAME.second);
static PRM_Name namePoMode(POMODE_NAME.first, POMODE_NAME.second);
static PRM_Name namePoLineRotate(POROTATE_NAME.first, POROTATE_NAME.second);

/// Define node-specific ranges
static PRM_Range rangePoRotate
{
	PRM_RangeFlag::PRM_RANGE_UI, -180,
	PRM_RangeFlag::PRM_RANGE_UI, 180
};

/// ChoiceList Lists
static PRM_Name poModeMenuNames[] =
{
	PRM_Name("point", "Point"),
	PRM_Name("line", "Line"),
	PRM_Name(0)
};

static PRM_ChoiceList poModeMenu
(
(PRM_ChoiceListType)(PRM_CHOICELIST_EXCLUSIVE | PRM_CHOICELIST_REPLACE),
::poModeMenuNames
);

/// Create Template List
PRM_Template
COP2_Pickover::myTemplateList[]
{
	TEMPLATE_SWITCHER,
	TEMPLATES_XFORM,
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepA),
	TEMPLATES_MANDELBROT,
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepB),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &namePoMode, PRMzeroDefaults, &poModeMenu),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &namePoPoint, PRMzeroDefaults),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &namePoLineRotate, PRMzeroDefaults, 0, &rangePoRotate),
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

	XformStashData xformData;
	xformData.evalArgs(this, t);
	data->space.set_xform(xformData);

	PickoverStashData pickoverData;
	pickoverData.evalArgs(this, t);
	data->fractal = Pickover(pickoverData);

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
			// TODO: Make optional in interface
			else if (tileIndex == 1)  // Reference Lines
			{
				// TODO: Move into its own method and encapsulate code
				auto world_point = data->space.get_pixel_coords(data->fractal.data.popoint);

				double distance = SYSsqrt(
					SYSpow(world_point.first - worldPixel.first, 2.0) +
					SYSpow(world_point.second - worldPixel.second, 2.0));

				// TODO: Move '10' to a parm
				if (distance < 10.0)
					dest[i] = (10.0 - distance) / 10.0;
				else
					dest[i] = 0.0f;

				// TODO: Move into the fractal class
				if (data->fractal.data.pomode)
				{
					distance = data->fractal.distance_to_line(
						fractalCoords,
						data->fractal.data.popoint,
						data->fractal.data.porotate);

					// TODO: Move this all to a parm
					if (distance < .1)
						dest[i] = (.1 - distance) * 10;
					else
						dest[i] = 0.0f;
				}
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
	int mode = evalInt(namePoMode.getToken(), 0, t);
	
	// Set variables for hiding
	bool displayRotate{ false };

	if (mode == 1)
		displayRotate = true;

	// Call parent's updateParmFlags to avoid recursion.
	bool changed = COP2_Generator::updateParmsFlags();

	changed |= setVisibleState(namePoLineRotate.getToken(), displayRotate);

	return changed;
}

/// Destructor
COP2_Pickover::~COP2_Pickover() {}

/// Destructor
COP2_PickoverData::~COP2_PickoverData() {}