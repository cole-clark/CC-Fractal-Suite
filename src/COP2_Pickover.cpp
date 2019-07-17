/*
	Cole Clark's Fractal Suite

	COP2_Pickover.cpp
	Code for CC Pickover Fracatal Generator Cop Node.
 */

#include "COP2_Pickover.h"

using namespace CC;

/// Parm Switcher used by this interface
COP_GENERATOR_SWITCHER(15, "Fractal");

 /// Private Constructor
COP2_Pickover::COP2_Pickover(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry) : COP2_Generator(parent, name, entry) {}

/// Define node-specific attributes
static PRM_Name namePickoverPoint("pickover_point", "Pickover Offset");
static PRM_Name namePickoverMode("pickover_mode", "Pickover Use Line");
static PRM_Name namePickoverLineRotate("pickover_line_rotate", "Pickover Line Rotate");

/// Create Template List
PRM_Template
COP2_Pickover::myTemplateList[]
{
	TEMPLATE_SWITCHER,
	TEMPLATES_XFORM,
	MAKE_SEP_TEMPLATE(1),
	TEMPLATES_MANDELBROT,
	MAKE_SEP_TEMPLATE(2),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &namePickoverPoint, PRMzeroDefaults),
	PRM_Template(PRM_TOGGLE_J, TOOL_PARM, 1, &namePickoverMode, PRMzeroDefaults),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &namePickoverLineRotate, PRMzeroDefaults),
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
	
	data->space.set_image_size(image_sizex, image_sizey);

	XformStashData xformData;
	xformData.evalArgs(this, t);
	data->space.set_xform(xformData);

	MandelbrotStashData mandelData;
	mandelData.evalArgs(this, t);

	/*

	// Set the size of the fractal space relative to this context's size.
	


	// Sets the base xform of the fractal from the interface that will be calculated by
	// The pixels.
	data->space.set_xform(
		offset_x,
		offset_y,
		rotate,
		scale,
		scale,
		rotatePivot_x,
		rotatePivot_y,
		scalePivot_x,
		scalePivot_y,
		xOrd);

	// Fractal Attributes
	int iter = evalInt(nameIter.getToken(), 0, t);
	double pow = evalFloat(namePow.getToken(), 0, t);
	double bailout = evalFloat(nameBailout.getToken(), 0, t);
	int jdepth = evalInt(nameJDepth.getToken(), 0, t);
	double joffset_x = evalFloat(nameJOffset.getToken(), 0, t);
	double joffset_y = evalFloat(nameJOffset.getToken(), 1, t);
	int blackhole = evalInt(nameBlackhole.getToken(), 0, t);

	data->fractal = Mandelbrot(
		iter, pow, bailout, jdepth, joffset_x, joffset_y, blackhole);
	*/
	return data;
}

/// Creates the image. This is called by multiple worker threads by Houdini.
OP_ERROR
COP2_Pickover::generateTile(COP2_Context& context, TIL_TileList* tileList)
{
	COP2_PickoverData* data{ static_cast<COP2_PickoverData*>(context.data()) };

	// Initialize float array the size of current tile list for values to write to.
	float *dest = new float[tileList->mySize]{ 0 };
	/*
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
				//dest[i] = pixelInfo.num_iter;
				dest[i] = pixelInfo.smooth;
			}
			else
				dest[i] = 0.0f;
		}

		writeFPtoTile(tileList, dest, tileIndex);
	};
	*/
	delete[] dest;

	return error();
}

/// Destructor
COP2_Pickover::~COP2_Pickover() {}

/// Destructor
COP2_PickoverData::~COP2_PickoverData() {}