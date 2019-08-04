/*
	Cole Clark's Fractal Suite

	COP2_Buddhabrot.cpp
	Code for CC Buddhabrot Generator Cop Node.
 */

#include <random>

#include <CH/CH_Manager.h>
#include <COP2/COP2_CookAreaInfo.h>

#include "COP2_Buddhabrot.h"

using namespace CC;

COP_MASK_SWITCHER(19, "Fractal");

/// Declare Parm Names

static PRM_Name nameSamples("samples", "Samples");
static PRM_Name nameSeed("seed", "Seed");
static PRM_Name nameNormalize("normalize", "Normalize");
static PRM_Name nameMaxval("maxval", "Maximum Value");
static PRM_Name nameDisplayReferenceFractal("displayreffractal", "Display Reference Fractal");

/// Declare Parm Defaults

static PRM_Default defaultSamples{ 0.25 };  // Sample by 25% of image size.
static PRM_Default defaultMaxval{ -1 };  // Off by default

/// Deflare Parm Ranges
static PRM_Range rangeSamples
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0.01,
	PRM_RangeFlag::PRM_RANGE_UI, 5
};

static PRM_Range rangeMaxval
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, -1,
	PRM_RangeFlag::PRM_RANGE_UI, 100
};

/// Create Template List
PRM_Template
COP2_Buddhabrot::myTemplateList[]
{
	// The Cop2 generator defaults to having 3 tabs: Mask, Image, Sequence. +1 for ours.
	PRM_Template(PRM_SWITCHER, 3, &PRMswitcherName, switcher),
	TEMPLATES_XFORM,
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepA),
	TEMPLATES_MANDELBROT,
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepB),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameSamples, &defaultSamples, 0, &rangeSamples),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameSeed, PRMzeroDefaults),
	PRM_Template(PRM_TOGGLE_J, TOOL_PARM, 1, &nameNormalize, PRMoneDefaults),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameMaxval, &defaultMaxval, 0, &rangeMaxval),
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepC),
	PRM_Template(PRM_TOGGLE_J, TOOL_PARM, 1, &nameDisplayReferenceFractal, PRMoneDefaults),
	PRM_Template()
};

OP_TemplatePair COP2_Buddhabrot::myTemplatePair(
	COP2_Buddhabrot::myTemplateList,
	&COP2_MaskOp::myTemplatePair);

OP_VariablePair COP2_Buddhabrot::myVariablePair(
	0,
	&COP2_MaskOp::myVariablePair);

const char* COP2_Buddhabrot::myInputLabels[] =
{
	"Image to Filter",
	"Mask Input",
	0
};

COP2_Buddhabrot::COP2_Buddhabrot(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry)
	: COP2_MaskOp(parent, name, entry)
{
	// Node affects (C, A, All Extra Image Planes)
	setDefaultScope(true, true, 0);
}

COP2_Buddhabrot::~COP2_Buddhabrot() {}

COP2_ContextData *
COP2_Buddhabrot::newContextData(
	const TIL_Plane* /*plane*/,
	int /*arrayindex*/,
	float t,
	int image_sizex, int image_sizey,
	int /*thread*/, int /*maxthreads*/)
{
	// Stash Node Parms into this data object and return
	COP2_BuddhabrotData *data = new COP2_BuddhabrotData();

	data->space.set_image_size(image_sizex, image_sizey);

	XformStashData xformData;
	xformData.evalArgs(this, t);
	data->space.set_xform(xformData);

	MandelbrotStashData mandelData;
	mandelData.evalArgs(this, t);
	data->fractal = Mandelbrot(mandelData);

	// Node-Specific Parms

	data->samples = evalFloat(nameSamples.getToken(), 0, t);
	data->seed = evalInt(nameSeed.getToken(), 0, t);
	data->normalize = evalInt(nameNormalize.getToken(), 0, t);
	data->maxval = evalInt(nameMaxval.getToken(), 0, t);
	data->displayreffractal = evalInt(nameDisplayReferenceFractal.getToken(), 0, t);

	return data;
}

void
COP2_Buddhabrot::computeImageBounds(COP2_Context &context)
{
	context.setImageBounds(0, 0, context.myXres - 1, context.myYres - 1);
	// In theory `copyInputBounds(0, context);` should work, but it
	// results in a black image in 17.5.
}

bool COP2_Buddhabrot::updateParmsFlags()
{
	// Determine If Normalizing
	fpreal t = CHgetEvalTime();
	bool normalize = evalInt(nameNormalize.getToken(), 0, t);

	// Set variables for hiding
	bool displayMaxval{ false };

	if (normalize)
	{
		displayMaxval = true;
	}

	// Set the visibility state for hidable parms.
	bool changed = COP2_MaskOp::updateParmsFlags();

	changed |= setVisibleState(nameMaxval.getToken(), displayMaxval);

	return changed;
}

void
COP2_Buddhabrot::getInputDependenciesForOutputArea(
	COP2_CookAreaInfo& output_area,
	const COP2_CookAreaList& input_areas,
	COP2_CookAreaList& needed_areas)
{
	COP2_CookAreaInfo* area;

	area = makeOutputAreaDependOnInputPlane(0,
		output_area.getPlane().getName(),
		output_area.getArrayIndex(),
		output_area.getTime(),
		input_areas, needed_areas);

	// Set All of Input Area to Output area
	if (area)
		area->enlargeNeededAreaToBounds();

	getMaskDependency(output_area, input_areas, needed_areas);
}

std::vector<COMPLEX>
COP2_Buddhabrot::buddhabrotPoints(
	Mandelbrot* fractal, const COMPLEX & c, int nIterations)
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
COP2_Buddhabrot::doCookMyTile(COP2_Context& context, TIL_TileList* tiles)
{
	COP2_BuddhabrotData* sdata =
		static_cast<COP2_BuddhabrotData*>(context.data());

	// Convenience Method that cooks the entire image.
	return cookFullImage(
		context,
		tiles,
		&COP2_Buddhabrot::filter,
		sdata->myLock, true);
}

OP_ERROR
COP2_Buddhabrot::filter(
	COP2_Context& context,
	const TIL_Region* input,
	TIL_Region* output,
	COP2_Node* me)
{
	// Avoid having to write pointer-references (me->...)
	// in the filter by passing these
	// Args to a static version of this function prototype.
	return ((COP2_Buddhabrot*)me)->filterImage(context, input, output);
}

OP_ERROR
COP2_Buddhabrot::filterImage(
	COP2_Context& context,
	const TIL_Region* input,
	TIL_Region* output)
{
	// Get the context data
	COP2_BuddhabrotData* sdata =
		(COP2_BuddhabrotData *)context.data();

	int comp;
	int x, y;
	char *idata, *odata;

	std::mt19937 rng;
	rng.seed(sdata->seed);

	// Scale num of samples to the size of the image.
	exint num_samples = SYSrint(context.myXsize * context.myYsize * sdata->samples);

	uint32_t highest_sample_value;

	// Declare reference fractal (with lower iteration count) if requested.
	Mandelbrot refFractal;
	if (sdata->displayreffractal)
	{
		refFractal = sdata->fractal;
		refFractal.data.iters = REFERENCE_FRACTAL_ITERS;
	}

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

				for (exint idxSample = 0; idxSample < num_samples; idxSample++)
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
				// Normalize to highest sample value if needed
				if (sdata->normalize)
				{
					// Get the lower value, so that normalize always fits 0-1.
					highest_sample_value = (sdata->maxval < highest_sample_value ?
						sdata->maxval : highest_sample_value);

					float multiplier = 1.0f / highest_sample_value;
					for (int x = 0; x < context.myXsize; ++x)
					{
						for (int y = 0; y < context.myYsize; ++y)
						{
							int currentPixel = x + y * context.myXsize;
							float* outputPixel = (float*)odata;
							outputPixel += currentPixel;

							// Clamp maximum pixel value if maxval is not -1
							if (sdata->maxval > -1 && *outputPixel > sdata->maxval)
								*outputPixel = sdata->maxval;
							*outputPixel *= multiplier;
						}
					}
				}
			}
			// Display reference fractal in second image plane
			else if (comp == 1 && sdata->displayreffractal)
			{
				for (int x = 0; x < context.myXsize; ++x)
				{
					for (int y = 0; y < context.myYsize; ++y)
					{
						int currentPixel = x + y * context.myXsize;
						float *outputPixel = (float *)odata;
						outputPixel += currentPixel;
						COMPLEX fractalCoords = sdata->space.get_fractal_coords(WORLDPIXELCOORDS(x, y));

						// Assign as a normalized value
						*outputPixel = refFractal.calculate(fractalCoords).num_iter /
									   (float)refFractal.data.iters;
					}
				}
			}
		}
	}

	// TODO: test deleting sdata.
	return error();
}