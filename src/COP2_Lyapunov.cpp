/** \file COP2_Lyapunov.cpp
	Source declaring the Lyapunov Cop2 Operator.
 */

 // Local
#include "COP2_Lyapunov.h"
#include "FractalNode.h"

COP_GENERATOR_SWITCHER(10, "Fractal");


CC::COP2_Lyapunov::COP2_Lyapunov(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry) : COP2_Generator(parent, name, entry) {}

TIL_Sequence*
CC::COP2_Lyapunov::cookSequenceInfo(OP_ERROR & error)
{
	// Fetch sequence info from parent class.
	COP2_Generator::cookSequenceInfo(error);

	return &mySequence;
}


COP2_ContextData*
CC::COP2_Lyapunov::newContextData

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


OP_TemplatePair
CC::COP2_Lyapunov::myTemplatePair
(
	COP2_Lyapunov::myTemplateList,
	&COP2_Generator::myTemplatePair
);


OP_VariablePair
CC::COP2_Lyapunov::myVariablePair
(
	0,
	&COP2_Node::myVariablePair
);

PRM_Template
CC::COP2_Lyapunov::myTemplateList[]
{
	TEMPLATE_SWITCHER,
	TEMPLATES_XFORM,
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepA),
	TEMPLATES_LYAPUNOV,
	PRM_Template()
};

// Creates the image. This is called by multiple worker threads by Houdini.
OP_ERROR
CC::COP2_Lyapunov::generateTile(COP2_Context& context, TIL_TileList* tileList)
{
	COP2_LyapunovData* data{ static_cast<COP2_LyapunovData*>(context.data()) };

	// Initialize float array the size of current tile list for values
	// to write to.
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

		for (exint i = 0; i < size_x * size_y; i++)
		{
			if (tileIndex == 0)
			{
				WORLDPIXELCOORDS worldPixel = CC::calculate_world_pixel(
					tileList, tile, i);
				COMPLEX fractalCoords = data->space.get_fractal_coords(
					worldPixel);
				FractalCoordsInfo pixelInfo = data->fractal.calculate(
					fractalCoords);

				dest[i] = pixelInfo.smooth;
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
