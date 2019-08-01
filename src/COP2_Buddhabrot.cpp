/*
	Cole Clark's Fractal Suite

	COP2_Buddhabrot.cpp
	Code for CC Buddhabrot Generator Cop Node.
 */

#include <random>

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

COP_MASK_SWITCHER(18, "Fractal");


/// Declare Parm Names
static PRM_Name nameScale("scale", "Scale");
static PRM_Name nameOffset("offset", "Offset");
static PRM_Name nameRotate("rotate", "Rotate");
static PRM_Name nameXOrd("xOrd", "Xform Order");
static PRM_Name nameIter("iter", "Iterations");
static PRM_Name namePow("pow", "Exponent");
static PRM_Name nameBailout("bailout", "Bailout");
static PRM_Name nameJDepth("jdepth", "Julia Depth");
static PRM_Name nameJOffset("joffset", "Julia Offset");
static PRM_Name nameBlackhole("blackhole", "Blackhole");
static PRM_Name nameSep1("sep1", "sep1");
static PRM_Name nameSep2("sep2", "sep2");
static PRM_Name nameSep3("sep3", "sep3");
static PRM_Name nameSep4("sep4", "sep4");
static PRM_Name nameRotatePivot("rpivot", "Rotate Pivot");
static PRM_Name nameScalePivot("spivot", "Scale Pivot");

static PRM_Name nameSamples("samples", "Samples");
static PRM_Name nameSeed("seed", "Seed");


/// ChoiceList Lists
static PRM_Name xordMenuNames[] =
{
	PRM_Name("TRS", "Translate Rotate Scale"),
	PRM_Name("TSR", "Translate Scale Rotate"),
	PRM_Name("RTS", "Rotate Translate Scale"),
	PRM_Name("RST", "Rotate Scale Translate"),
	PRM_Name("STR", "Scale Translate Rotate"),
	PRM_Name("SRT", "Scale Rotate Translate"),
	PRM_Name(0)
};

static PRM_ChoiceList xOrdMenu
(
(PRM_ChoiceListType)(PRM_CHOICELIST_EXCLUSIVE | PRM_CHOICELIST_REPLACE),
::xordMenuNames
);

/// Declare Parm Defaults
static PRM_Default defaultScale{ 500000 };
static PRM_Default defaultIter{ 50 };
static PRM_Default defaultPow{ 2 };
static PRM_Default defaultBailout{ 4 };  // 4 Looks good at 4k when smoothing.
static PRM_Default defaultXOrd{ 5 };  // Scale Rotate Translate
static PRM_Default defaultOffset[] = { -1000, -750 };
static PRM_Default defaultRotatePivot[] = { 0.5, 0.5 };
static PRM_Default defaultScalePivot[] = { 0.5, 0.5 };
static PRM_Default defaultSamples{ 100 };

/// Deflare Parm Ranges
static PRM_Range rangeScale
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, defaultScale.getFloat()
};

static PRM_Range rangeRotate
{
	PRM_RangeFlag::PRM_RANGE_UI, -180,
	PRM_RangeFlag::PRM_RANGE_UI, 180
};

static PRM_Range rangeIter
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 1,
	PRM_RangeFlag::PRM_RANGE_UI, 200
};


static PRM_Range rangePow
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, 10
};

static PRM_Range rangeBailout
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, 4
};

static PRM_Range rangeJDepth
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, 5
};

static PRM_Range rangeSamples
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 1,
	PRM_RangeFlag::PRM_RANGE_UI, 1000
};

/// Create Template List
PRM_Template
COP2_Buddhabrot::myTemplateList[]
{
	// The Cop2 generator defaults to having 3 tabs: Mask, Image, Sequence. +1 for ours.
	PRM_Template(PRM_SWITCHER, 4, &PRMswitcherName, switcher),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameXOrd, &defaultXOrd, &xOrdMenu),
	PRM_Template(PRM_FLT_LOG, TOOL_PARM, 1, &nameScale, &defaultScale, 0, &rangeScale),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameOffset, defaultOffset),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameRotate, PRMzeroDefaults, 0, &rangeRotate),
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSep1, PRMzeroDefaults),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameRotatePivot, defaultRotatePivot),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameScalePivot, defaultScalePivot),
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSep2, PRMzeroDefaults),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameIter, &defaultIter, 0, &rangeIter),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &namePow, &defaultPow, 0, &rangePow),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameBailout, &defaultBailout, 0, &rangeBailout),
	PRM_Template(PRM_TOGGLE_J, TOOL_PARM, 1, &nameBlackhole, PRMzeroDefaults),
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSep3, PRMzeroDefaults),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameJDepth, PRMzeroDefaults, 0, &rangeJDepth),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameJOffset, PRMzeroDefaults),
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSep4, PRMzeroDefaults),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameSamples, &defaultSamples, 0, &rangeSamples),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameSeed, PRMzeroDefaults),
	PRM_Template()
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
	float t, int xres, int yres,
	int /*thread*/, int /*maxthreads*/)
{
	// This method evaluates and stashes parms and any other data that
	// needs to be setup. Parms cannot be evaluated concurently in separate
	// threads. This function is guaranteed to be single threaded.
	COP2_BuddhabrotData *data = new COP2_BuddhabrotData();

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

	data->space.set_image_size(xres, yres);
	data->space.set_xform(
		offset_x,
		offset_y,
		rotate,
		scale,
		scale,
		xOrd);


	// Fractal Attributes
	int iter = evalInt(nameIter.getToken(), 0, t);
	double pow = evalFloat(namePow.getToken(), 0, t);
	double bailout = evalFloat(nameBailout.getToken(), 0, t);
	int jdepth = evalInt(nameJDepth.getToken(), 0, t);
	double joffset_x = evalFloat(nameJOffset.getToken(), 0, t);
	double joffset_y = evalFloat(nameJOffset.getToken(), 1, t);
	int blackhole = evalInt(nameBlackhole.getToken(), 0, t);

	COMPLEX joffset{ joffset_x, joffset_y };

	data->fractal = Mandelbrot(
		iter, pow, bailout, jdepth, joffset, blackhole);


	exint samples = evalInt(nameSamples.getToken(), 0, t);
	int seed = evalFloat(nameSeed.getToken(), 0, t);

	data->samples = samples;
	data->seed = seed;


	return data;
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
std::vector<COMPLEX> CC::COP2_Buddhabrot::buddhabrotPoints(Mandelbrot* fractal, const COMPLEX & c, int nIterations)
{
	std::vector<COMPLEX> points;
	points.reserve(nIterations);

	COMPLEX z{ 0 };
	int n{ 0 };

	while (n < nIterations)
	{
		++n;
		z = fractal->calculate_z(z, c);

		if (abs(z) > fractal->data.bailout)
			break;

		points.push_back(z);

	};

	// Return nothing if the point is bounded within the mandelbrot set
	if (fractal->data.blackhole)
	{
		if (n == nIterations)
			return std::vector<COMPLEX>();
	}

	return points;
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

	std::mt19937 rng;
	rng.seed(sdata->seed);

	uint32_t highest_sample_value;

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
			if (comp == 0) // First plane only
			{
				// Choose a random x, y coordinate along the image plane.
				// The '0's refer to lower left corner, the second argument the upper right
				std::uniform_real_distribution<double> realDistribution(0, context.myXsize - 1);
				std::uniform_real_distribution<double> imagDistribution(0, context.myYsize - 1);

				for (exint idxSample = 0; idxSample < sdata->samples; idxSample++)
				{
					COMPLEX sample(realDistribution(rng), imagDistribution(rng));
					COMPLEX fractalCoords = sdata->space.get_fractal_coords(sample);

					std::vector<COMPLEX> points = buddhabrotPoints(&sdata->fractal, fractalCoords, sdata->fractal.data.iters);

					for (COMPLEX& point : points)
					{
						float *outputPixel = (float *)odata;

						WORLDPIXELCOORDS samplePixelCoords = sdata->space.get_pixel_coords(point);
						int samplePixel = samplePixelCoords.first + (samplePixelCoords.second * context.myXsize);

						if (samplePixel >= 0 && samplePixel <= context.myXsize * context.myYsize)
						{
							outputPixel += samplePixel;
							++*outputPixel;

							// Save the highest output pixel value sampled
							if (*outputPixel > highest_sample_value)
								highest_sample_value = static_cast<uint32_t>(*outputPixel);
						}
					}
				}
			}
			else if (comp == 1)  // Test show mandelbrot in second pane
			{
				for (int x = 0; x < context.myXsize; ++x)
				{
					for (int y = 0; y < context.myYsize; ++y)
					{
						int currentPixel = x + y * context.myXsize;
						float *outputPixel = (float *)odata;
						outputPixel += currentPixel;
						COMPLEX fractalCoords = sdata->space.get_fractal_coords(WORLDPIXELCOORDS(x, y));

						*outputPixel = sdata->fractal.calculate(fractalCoords).num_iter;
					}
				}
			}
			// TEMP: stores max value into the entire image. Beg SideFx to let me put in image metadata
			else if (comp == 2)
			{
				for (int x = 0; x < context.myXsize; ++x)
					for (int y = 0; y < context.myYsize; ++y)
					{
						int currentPixel = x + y * context.myXsize;
						float* outputPixel = (float*)odata;
						outputPixel += currentPixel;
						*outputPixel = highest_sample_value;
					}
			}
		}
	}
	// TODO: test deleting sdata.
	return error();
}