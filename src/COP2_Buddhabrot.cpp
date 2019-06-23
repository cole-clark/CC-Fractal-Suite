/*
	Cole Clark's Fractal Suite

	COP2_Buddhabrot.cpp
	Code for CC Buddhabrot Generator Cop Node.
 */

#include <OP/OP_Context.h>
#include <OP/OP_OperatorTable.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Floor.h>
#include <PRM/PRM_Include.h>
#include <PRM/PRM_Parm.h>
#include <TIL/TIL_Region.h>
#include <TIL/TIL_Plane.h>
#include <TIL/TIL_Sequence.h>
#include <TIL/TIL_Tile.h>
#include <COP2/COP2_CookAreaInfo.h>
#include "COP2_Buddhabrot.h"

using namespace CC;

COP_MASK_SWITCHER(1, "Sample Full Image Filter");
static PRM_Name names[] =
{
	PRM_Name("size",    "Size"),
};
static PRM_Default sizeDef(10);
static PRM_Range sizeRange(PRM_RANGE_UI, 0, PRM_RANGE_UI, 100);
PRM_Template
COP2_Buddhabrot::myTemplateList[] =
{
	PRM_Template(PRM_SWITCHER,  3, &PRMswitcherName, switcher),
	PRM_Template(PRM_FLT_J,     TOOL_PARM, 1, &names[0], &sizeDef, 0,
				 &sizeRange),
	PRM_Template(),
};
OP_TemplatePair COP2_Buddhabrot::myTemplatePair(
	COP2_Buddhabrot::myTemplateList,
	&COP2_MaskOp::myTemplatePair);
OP_VariablePair COP2_Buddhabrot::myVariablePair(0,
	&COP2_MaskOp::myVariablePair);
const char *    COP2_Buddhabrot::myInputLabels[] =
{
	"Image to Filter",
	"Mask Input",
	0
};
OP_Node *
COP2_Buddhabrot::myConstructor(OP_Network      *net,
	const char      *name,
	OP_Operator     *op)
{
	return new COP2_Buddhabrot(net, name, op);
}
COP2_Buddhabrot::COP2_Buddhabrot(OP_Network *parent,
	const char *name,
	OP_Operator *entry)
	: COP2_MaskOp(parent, name, entry)
{
	// sets the default scope to only affect color and alpha. The global
	// default is 'true, true, "*"', which affects color, alpha and all
	// extra planes.
	setDefaultScope(true, true, 0);
}
COP2_Buddhabrot::~COP2_Buddhabrot()
{
	;
}
// -----------------------------------------------------------------------
COP2_ContextData *
COP2_Buddhabrot::newContextData(const TIL_Plane * /*plane*/,
	int /*arrayindex*/,
	float t, int xres, int /*yres*/,
	int /*thread*/, int /*maxthreads*/)
{
	// This method evaluates and stashes parms and any other data that
	// needs to be setup. Parms cannot be evaluated concurently in separate
	// threads. This function is guaranteed to be single threaded.
	COP2_BuddhabrotData *sdata = new COP2_BuddhabrotData();
	int index = mySequence.getImageIndex(t);
	// xres may not be the full image res (if cooked at 1/2 or 1/4). Because
	// we're dealing with a size, scale down the size based on our res.
	// getXScaleFactor will return (xres / full_xres). 
	sdata->mySize = SIZE(t) * getXScaleFactor(xres)*getFrameScopeEffect(index);

	return sdata;
}
void
COP2_Buddhabrot::computeImageBounds(COP2_Context &context)
{
	// if your algorthim increases the image bounds (like blurring or
	// transforming) you can set the bounds here.
	// if you need to access your context data for some information to
	// compute the bounds (like blur size), you can do it like:
	//   COP2_BuddhabrotData *sdata =
	//                (COP2_BuddhabrotData *) context.data();
	// SAMPLES:

	// expands or contracts the bounds to the visible image resolution
	context.setImageBounds(0, 0, context.myXres - 1, context.myYres - 1);
	// just copies the input bounds (ie this node don't modify it)
	//copyInputBounds(0, context);
	// expands the input bounds by 5 pixels in each direction.
	//   copyInputBounds(0, context);
	//   int x1,y1,x2,y2;
	//   context.getImageBounds(x1,y1,x2,y2);
	//   context.setImageBounds(x1-5, y1-5, x2+5, y2+5);

}
void
COP2_Buddhabrot::getInputDependenciesForOutputArea(
	COP2_CookAreaInfo           &output_area,
	const COP2_CookAreaList     &input_areas,
	COP2_CookAreaList           &needed_areas)
{
	COP2_CookAreaInfo   *area;
	// for a given output area and plane, set up which input planes and areas
	// it is dependent on. Basically, if you call inputTile or inputRegion in
	// the cook, for each call you need to make a dependency here.
	// this makes a dependency on the input plane corresponding to the output
	// area's plane. 
	area = makeOutputAreaDependOnInputPlane(0,
		output_area.getPlane().getName(),
		output_area.getArrayIndex(),
		output_area.getTime(),
		input_areas, needed_areas);

	// Always check for null before setting the bounds of the input area.
	// in this case, all of the input area is required.
	if (area)
		area->enlargeNeededAreaToBounds();
	// If the node depends on its input counterpart PLUS another plane,
	// we need to add a dependency on that plane as well. In this case, we
	// add an extra dependency on alpha (same input, same time).
	area = makeOutputAreaDependOnInputPlane(0,
		getAlphaPlaneName(), 0,
		output_area.getTime(),
		input_areas, needed_areas);
	// again, we'll use all of the area.
	if (area)
		area->enlargeNeededAreaToBounds();
	getMaskDependency(output_area, input_areas, needed_areas);

}
OP_ERROR
COP2_Buddhabrot::doCookMyTile(COP2_Context &context, TIL_TileList *tiles)
{
	// normally, this is where you would process your tile. However,
	// cookFullImage() is a convenience function which assembles a full image
	// and does all the proper locking for you, then calls your filter
	// function.

	COP2_BuddhabrotData *sdata =
		static_cast<COP2_BuddhabrotData *>(context.data());
	return cookFullImage(context, tiles, &COP2_Buddhabrot::filter,
		sdata->myLock, true);
}
OP_ERROR
COP2_Buddhabrot::filter(COP2_Context &context,
	const TIL_Region *input,
	TIL_Region *output,
	COP2_Node  *me)
{
	// since I don't like typing me-> constantly, just call a member function
	// from this static function. 
	return ((COP2_Buddhabrot*)me)->filterImage(context, input, output);
}
OP_ERROR
COP2_Buddhabrot::filterImage(COP2_Context &context,
	const TIL_Region *input,
	TIL_Region *output)
{
	// retrieve my context data information (built in newContextData).
	COP2_BuddhabrotData *sdata =
		(COP2_BuddhabrotData *)context.data();

	int comp;
	int x, y;
	char *idata, *odata;

	// For each image plane.
	for (comp = 0; comp < PLANE_MAX_VECTOR_SIZE; comp++)
	{
		// For each plane, calculate input and output data
		idata = (char *)input->getImageData(comp);
		odata = (char *)output->getImageData(comp);

		if (odata)
		{
			// since we aren't guarenteed to write to every pixel with this
			// 'algorithm', the output data array needs to be zeroed. 
			memset(odata, 0, context.myXsize*context.myYsize * sizeof(float));
		}
		if (idata && odata)
		{
			// myXsize & myYsize are the actual sizes of the large canvas,
			// which may be different from the resolution (myXres, myYres).
			for (y = 0; y < context.myYsize; y++)
				for (x = 0; x < context.myXsize; x++)
				{
					// Set the current pixel
					int currentPixel = x + y * context.myXsize;
					// Declare output data and set to evaluate every pixel
					float *outputPixel = (float *)odata;
					outputPixel += (currentPixel);
					
					// Declare the value of the pixel
					float *inputPixel = (float *)idata;
					inputPixel += (currentPixel);  // Set pointer to current pixel
					*inputPixel += 0.5;  // Add to value from input

					// Add pix value to output position
					*outputPixel = *outputPixel + *inputPixel;

				}
		}
	}

	return error();
}