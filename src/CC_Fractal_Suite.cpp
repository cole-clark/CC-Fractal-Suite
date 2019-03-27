// Defines the version information for every DSO, initializes function prototypes spec
#include <UT/UT_DSOVersion.h>

#include <sys/SYS_Math.h>
#include <UT/UT_SysClone.h>

#include <OP/OP_OperatorTable.h>

#include <PRM/PRM_Include.h>
#include <PRM/PRM_Parm.h>
#include <PRM/PRM_ChoiceList.h>

#include <TIL/TIL_Plane.h>
#include <TIL/TIL_Tile.h>

#include "CC_Fractal_Suite.h"


using namespace CC;

COP_GENERATOR_SWITCHER(8, "Fractal");

static PRM_Name names[]
{
	PRM_Name("mode", "Mode"),
	PRM_Name("scale", "Scale"),
	PRM_Name("offset", "Offset"),
	PRM_Name("maxiter", "Max Iterations"),
	PRM_Name("mandelbrot_pow", "Mandelbrot Pow"),
	PRM_Name("mandelbrot_max", "Mandelbrot Max"),
	PRM_Name("julia_offset_x", "Julia Offset X"),
	PRM_Name("julia_offset_y", "Julia Offset Y")
};

static PRM_Name modeMenuList[] =
{
	PRM_Name("mandelbrot", "Mandelbrot"),
	PRM_Name("brahmabrot", "Brahmabrot"),
	PRM_Name("starbrot", "Starbrot"),
	PRM_Name("Julia", "Julia"),
	PRM_Name("mandeltan", "Mandeltan"),
	PRM_Name("mandelbar", "Mandelbar"),
	PRM_Name("temp_2", "Temp 2"),
	PRM_Name("temp_3", "Temp 3"),
	PRM_Name("temp_4", "Temp 4"),
	PRM_Name("temp_5", "Temp 5"),
	PRM_Name("temp_6", "Temp 6"),
	PRM_Name(0)
};

// Menus
static PRM_ChoiceList modeMenu((PRM_ChoiceListType)
(PRM_CHOICELIST_EXCLUSIVE | PRM_CHOICELIST_REPLACE), ::modeMenuList);

// Defaults
static PRM_Default iterDefault{ 50 };
static PRM_Default zeroDefault{ 0 };
static PRM_Default offsetDefaults[] = { PRM_Default{0}, PRM_Default{0} };
static PRM_Default mandel_two{ 2 };

// Template List
PRM_Template
COP2_FractalGenerator::myTemplateList[]
{
	PRM_Template(PRM_SWITCHER, 3, &PRMswitcherName, switcher),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &names[0], &zeroDefault, &modeMenu),    // Mode
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &names[1], PRMoneDefaults),             // Scale
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &names[2], offsetDefaults),            // Offset
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &names[3], &iterDefault),               // Max Iter
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &names[4], &mandel_two),               // Mandelbrot Pow
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &names[5], &mandel_two),               // Mandelbrot max
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &names[6], PRMoneDefaults),             // Julia x
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &names[7], PRMoneDefaults),             // Julia y
	PRM_Template(),
};

OP_TemplatePair COP2_FractalGenerator::myTemplatePair
(
	COP2_FractalGenerator::myTemplateList,
	&COP2_Generator::myTemplatePair
);

OP_VariablePair COP2_FractalGenerator::myVariablePair
(
	0,
	&COP2_Node::myVariablePair
);

OP_Node *
COP2_FractalGenerator::myConstructor(
	OP_Network* net,
	const char* name,
	OP_Operator* op)
{
	return new COP2_FractalGenerator(net, name, op);
}

COP2_FractalGenerator::COP2_FractalGenerator(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry) : COP2_Generator(parent, name, entry) {}

COP2_FractalGenerator::~COP2_FractalGenerator() {}

TIL_Sequence*
COP2_FractalGenerator::cookSequenceInfo(OP_ERROR& error)
{
	COP2_Generator::cookSequenceInfo(error);

	return &mySequence;
}

float CC::COP2_FractalGenerator::get_iterations(double x, double y, int max_iterations, fpreal mandelpow, fpreal mandelmax, int mode, fpreal julia_offset_x, fpreal julia_offset_y, uint pixel_id)
{
	std::complex<double> z{ 0 };
	std::complex<double> c(x, y);

	float iterations{ 0 };
	
	while (iterations < max_iterations)
	{
		if (mode == 0)  // Mandelbrot
		{
			z = pow(z, mandelpow) + c;
			//z = z * z + c;

			if (abs(z) > mandelmax)
				break;
			iterations++;
		}
		else if (mode == 1)  // Brahmabrot
		{
			if (abs(z) < abs(c))
				z = pow(z, 4) + c;
			else if (abs(z) == (abs(c)))
				z = pow(z, 4);
			else
				z = pow(z, 4) - c;

			if (abs(z) > mandelmax)
				break;
			iterations++;
		}
		else if (mode == 2)  // Starbrot
		{
			z = z * 0.5 - pow(z, 7) + c;

			if (abs(z) > mandelmax)
				break;
			iterations++;
		}
		else if (mode == 3) // Julia
		{
			z = pow(z, mandelpow) + c;

			// The julia applies an iterated complex function equal to
			// newZ = pow(oldZ, 2) + c.
			std::complex<double> julia_offset{ julia_offset_x, julia_offset_y };

			z = pow(z, 2) + julia_offset;

			if (abs(z) > mandelmax)
				break;
			iterations++;
		}
		else if (mode == 4)  // Mandelbrot Tan
		{
			// Solid defautls are 0.005 scale, 2.0625 x, 0.1525 y.
			std::complex<double> julia_offset{ julia_offset_x, julia_offset_y  + c.imag()};
			z = -std::tan(pow(z, mandelpow) + c);
			//z = z * z + c;

			if (abs(z) > mandelmax)
				break;
			iterations++;
		}
		else if (mode == 5)  // Mandelbar
		{
			auto ztemp = z;
			double xtemp = pow(z.real(), mandelpow) - pow(z.imag(), mandelmax) + x;
			z = std::complex<double>(
				xtemp,
				-2 * ztemp.real() * ztemp.imag() + y);

			if (abs(z) > mandelmax)
				break;
			iterations++;
		}
		else if (mode == 6)  // Spiral Shaped Julia
		{
			typedef std::complex<double> scd;
			z = (pow(z, mandelpow) + pow(z, mandelpow + 2)) / (scd{ 2 } -pow(z, mandelpow) / 20.0) + c;

			if (abs(z) > mandelmax)
				break;
			iterations++;
		}
		else
			break;  // Escape
	}

	return iterations;
}

COP2_ContextData*
COP2_FractalGenerator::newContextData(
	const TIL_Plane* /*planename*/,
	int /*ArayIndex*/,
	float t, int /*xsize*/, int/*ysize*/,
	int /*thread*/, int /*max_num_threads*/)
{
	COP2_FractalGeneratorData* data{ new COP2_FractalGeneratorData };

	// Stash parm values to data object
	OFFSET(data->offset, t);
	data->scale = SCALE(t);
	data->maxiter = MAXITER(t);
	data->mode = MODE(t);
	data->mandelpow = MANDELPOW(t);
	data->mandelmax = MANDELMAX(t);
	data->julia_offset_x = JULIA_OFFSET_X(t);
	data->julia_offset_y = JULIA_OFFSET_Y(t);

	return data;
}

OP_ERROR
COP2_FractalGenerator::generateTile(COP2_Context& context, TIL_TileList* tiles)
{
	// Retrieve my context data created for this thread by newContextData method
	COP2_FractalGeneratorData* data{ static_cast<COP2_FractalGeneratorData*>(context.data()) };

	uint i;
	int ti;
	TIL_Tile* itr;
	unsigned seed;

	//const float* amp{data->myAmp.data() };
	//const float* mihoshi{ data->myMihoshi.data() };
	// Alloc some temp space for our values. If we know that we're always using FP [floating point?]
	// we could just write the values into the tiles directly using
	// dest = (float*) itr->getImageData();
	float *dest = new float[tiles->mySize]{0};

	// TileList, Tile, tile index
	FOR_EACH_UNCOOKED_TILE(tiles, itr, ti)
	{
		int size_x, size_y;
		itr->getSize(size_x, size_y);
		
		for (i = 0; i < size_x * size_y; i++)
		{
			if (ti == 0) // Red
			{
				auto tile_coords = data->calculate_coords(tiles, itr, i);
				double image_x = (tile_coords.first - context.myXsize / 2) * data->scale + data->offset.x();
				double image_y = (tile_coords.second - context.myYsize / 2) * data->scale + data->offset.y();
				dest[i] = this->get_iterations(image_x, image_y, data->maxiter, data->mandelpow, data->mandelmax, data->mode, data->julia_offset_x, data->julia_offset_y, i);
			}
			else
				dest[i] = 0;
		}

		writeFPtoTile(tiles, dest, ti);
	}

	delete[] dest;

	return error();
}

/// Install the cop.
void newCop2Operator(OP_OperatorTable* table)
{
	// This operator flags itself as a generator taking zero or one inputs.
	// The optional input is the mask.
	OP_Operator* op = new OP_Operator(
		"cc_fractal_suite_generator", // Node Name
		"CC Fractal Suite Generator", // Pretty Name
		COP2_FractalGenerator::myConstructor,
		&COP2_FractalGenerator::myTemplatePair,
		0,  // min inputs
		1,  // max inputs
		&COP2_FractalGenerator::myVariablePair,
		OP_FLAG_GENERATOR
		);

	table->addOperator(op);
}
