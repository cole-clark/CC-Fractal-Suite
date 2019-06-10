/*
	Cole Clark's Fractal Suite

	COP2_Buddhabrot.cpp
	Code for CC Buddhabrot Generator Cop node.
 */

#include "COP2_Buddhabrot.h"

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

using namespace CC;
COP_MASK_SWITCHER(1, "Sample Full Image Filter");
static PRM_Name names[] =
{
	PRM_Name("size",    "Size"),
};
static PRM_Default sizeDef(10);
static PRM_Range sizeRange(PRM_RANGE_UI, 0, PRM_RANGE_UI, 100);
PRM_Template
COP2_FullImageFilter::myTemplateList[] =
{
	PRM_Template(PRM_SWITCHER,  3, &PRMswitcherName, switcher),
	PRM_Template(PRM_FLT_J,     TOOL_PARM, 1, &names[0], &sizeDef, 0,
				 &sizeRange),
	PRM_Template(),
};
OP_TemplatePair COP2_FullImageFilter::myTemplatePair(
	COP2_FullImageFilter::myTemplateList,
	&COP2_MaskOp::myTemplatePair);
OP_VariablePair COP2_FullImageFilter::myVariablePair(0,
	&COP2_MaskOp::myVariablePair);
const char *    COP2_FullImageFilter::myInputLabels[] =
{
	"Image to Filter",
	"Mask Input",
	0
};
OP_Node *
COP2_FullImageFilter::myConstructor(OP_Network      *net,
	const char      *name,
	OP_Operator     *op)
{
	return new COP2_FullImageFilter(net, name, op);
}
COP2_FullImageFilter::COP2_FullImageFilter(OP_Network *parent,
	const char *name,
	OP_Operator *entry)
	: COP2_MaskOp(parent, name, entry)
{
	// sets the default scope to only affect color and alpha. The global
	// default is 'true, true, "*"', which affects color, alpha and all
	// extra planes.
	setDefaultScope(true, true, 0);
}
COP2_FullImageFilter::~COP2_FullImageFilter()
{
	;
}
// -----------------------------------------------------------------------
COP2_ContextData *
COP2_FullImageFilter::newContextData(const TIL_Plane * /*plane*/,
	int /*arrayindex*/,
	float t, int xres, int /*yres*/,
	int /*thread*/, int /*maxthreads*/)
{
	// This method evaluates and stashes parms and any other data that
	// needs to be setup. Parms cannot be evaluated concurently in separate
	// threads. This function is guaranteed to be single threaded.
	cop2_FullImageFilterData *sdata = new cop2_FullImageFilterData();
	int index = mySequence.getImageIndex(t);
	// xres may not be the full image res (if cooked at 1/2 or 1/4). Because
	// we're dealing with a size, scale down the size based on our res.
	// getXScaleFactor will return (xres / full_xres). 
	sdata->mySize = SIZE(t) * getXScaleFactor(xres)*getFrameScopeEffect(index);

	return sdata;
}
void
COP2_FullImageFilter::computeImageBounds(COP2_Context &context)
{
	// if your algorthim increases the image bounds (like blurring or
	// transforming) you can set the bounds here.
	// if you need to access your context data for some information to
	// compute the bounds (like blur size), you can do it like:
	//   cop2_FullImageFilterData *sdata =
	//                (cop2_FullImageFilterData *) context.data();
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
COP2_FullImageFilter::getInputDependenciesForOutputArea(
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
COP2_FullImageFilter::doCookMyTile(COP2_Context &context, TIL_TileList *tiles)
{
	// normally, this is where you would process your tile. However,
	// cookFullImage() is a convenience function which assembles a full image
	// and does all the proper locking for you, then calls your filter
	// function.

	cop2_FullImageFilterData *sdata =
		static_cast<cop2_FullImageFilterData *>(context.data());
	return cookFullImage(context, tiles, &COP2_FullImageFilter::filter,
		sdata->myLock, true);
}
OP_ERROR
COP2_FullImageFilter::filter(COP2_Context &context,
	const TIL_Region *input,
	TIL_Region *output,
	COP2_Node  *me)
{
	// since I don't like typing me-> constantly, just call a member function
	// from this static function. 
	return ((COP2_FullImageFilter*)me)->filterImage(context, input, output);
}
OP_ERROR
COP2_FullImageFilter::filterImage(COP2_Context &context,
	const TIL_Region *input,
	TIL_Region *output)
{
	// retrieve my context data information (built in newContextData).
	cop2_FullImageFilterData *sdata =
		(cop2_FullImageFilterData *)context.data();
	// currently we have a blank output region, and an input region filled with
	// whatever plane we've been told to cook. Both are in the same format, as
	// this node didn't alter the data formats of any planes.

	// we need the alpha plane, so grab it (generally, you'd want to check if
	// context.myPlane->isAlphaPlane() first, and then just use the 'input'
	// region if we were cooking alpha, but for simplicity's sake we won't
	// bother). Oh, and we'll grab it as floating point.
	// make a copy of the alpha plane & set it to FP format.
	TIL_Plane alphaplane(*mySequence.getPlane(getAlphaPlaneName()));
	alphaplane.setFormat(TILE_FLOAT32);
	alphaplane.setScoped(1);

	TIL_Region *alpha = inputRegion(0, context,    // input 0
		&alphaplane, 0, // FP alpha plane.
		context.getTime(), // at current cook time
		0, 0, // lower left corner
		context.myXsize - 1, context.myYsize - 1); //UR
	if (!alpha)
	{
		// something bad happened, possibly error, possibly user interruption.
		return UT_ERROR_ABORT;
	}
	int comp;
	int x, y;
	char *idata, *odata;
	float *adata;
	// my silly algorithm is as follows: it will take the value of the alpha
	// plane multiplied by the user defined size and move the source point
	// up to that distance away from its original location. It just adds the
	// pixel over any pixel at that location, for simplicities sake.
	adata = (float *)alpha->getImageData(0);

	// go component by component. PLANE_MAX_VECTOR_SIZE = 4.
	for (comp = 0; comp < PLANE_MAX_VECTOR_SIZE; comp++)
	{
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
					float *pix = (float *)idata;
					float *out = (float *)odata;
					unsigned seed = x * context.myYsize + y;
					float dx = SYSrandomZero(seed);
					float dy = SYSrandomZero(seed);
					int idx, idy;
					int nx, ny;
					dx *= adata[x + y * context.myXsize] * sdata->mySize;
					dy *= adata[x + y * context.myXsize] * sdata->mySize;
					idx = (int)SYSrint(dx);
					idy = (int)SYSrint(dy);
					nx = x + idx;
					ny = y + idy;
					if (nx < 0 || nx >= context.myXsize ||
						ny < 0 || ny >= context.myYsize)
						continue;
					pix += (x + y * context.myXsize);
					out += (nx + ny * context.myXsize);

					*out = *out + *pix;
				}
		}
	}

	// It is important to release regions and tiles you request with
	// inputRegion & inputTile, otherwise they will just sit around until the
	// end of the cook taking up memory. If someone puts down many of your
	// nodes in a network, this could be problematic.
	releaseRegion(alpha);
	// input and output are allocated & released by cookFullImage, so don't
	// release them.

	return error();
}