/*
	Cole Clark's Fractal Suite

	COP2_Pickover.cpp
	Code for CC Pickover Fracatal Generator Cop Node.
 */

#include "COP2_Pickover.h"

using namespace CC;

/// Parm Switcher used by this interface
COP_GENERATOR_SWITCHER(5, "Fractal");

 /// Private Constructor
COP2_Pickover::COP2_Pickover(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry) : COP2_Generator(parent, name, entry) {}

/*
/// Public Constructor
OP_Node *
COP2_Pickover::myConstructor(
	OP_Network* net,
	const char* name,
	OP_Operator* op)
{
	return new COP2_Pickover(net, name, op);
}*/

/// Create Template List
PRM_Template
COP2_Pickover::myTemplateList[]
{
	TEMPLATE_SWITCHER,
	TEMPLATES_XFORM,
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
	float t,           // Not actually sure, maybe tile?
	int image_sizex,         // xsize
	int image_sizey,         // ysize
	int,               // thread
	int                // max_num_threads
)
{
	// Create new empty data object.
	COP2_PickoverData* data{ new COP2_PickoverData };

	/*
	// Space Xform Attributes
	double scale = evalFloat(nameScale.getToken(), 0, t);
	double offset_x = evalFloat(nameOffset.getToken(), 0, t);
	double offset_y = evalFloat(nameOffset.getToken(), 1, t);
	const double rotate = evalFloat(nameRotate.getToken(), 0, t);
	const double rotatePivot_x = evalFloat(nameRotatePivot.getToken(), 0, t);
	const double rotatePivot_y = evalFloat(nameRotatePivot.getToken(), 1, t);
	const double scalePivot_x = evalFloat(nameScalePivot.getToken(), 0, t);
	const double scalePivot_y = evalFloat(nameScalePivot.getToken(), 1, t);

	const RSTORDER xOrd = get_rst_order(evalInt(nameXOrd.getToken(), 0, t));

	// In the houdini UI, it's annoying to type in really small numbers below 0.0001.
	// The UI artificially inflates the numbers to make them more user friendly at
	// shallow depths.
	scale = scale / 100000;  // This is set to make the default scale relative to 1e+5.
	offset_x = offset_x / 1000;
	offset_y = offset_y / 1000;

	// Set the size of the fractal space relative to this context's size.
	data->space.set_image_size(image_sizex, image_sizey);


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