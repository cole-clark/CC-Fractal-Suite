/*
	Cole Clark's Fractal Suite

	COP2_FractalMatte.h
	Code for CC Fractal Matte Cop filter node.
 */

#include <PRM/PRM_Include.h>
#include <CH/CH_Manager.h>

#include "COP2_FractalMatte.h"

#define MAX_BLEND_ITERATIONS 1000

using namespace CC;

typedef cop2_FractalMatteFunc::ModeType ModeType;
typedef cop2_FractalMatteFunc::ComparisonType ComparisonType;

COP_PIXEL_OP_SWITCHER(10, "Mattes");

/// Declare Parm Names
static PRM_Name nameModulo{ "modulo", "Modulo" };
static PRM_Name nameOffset{ "offset", "Offset" };
static PRM_Name nameInvert{ "invert", "Invert" };
static PRM_Name nameMode{ "mode", "Mode" };
static PRM_Name nameCompType{ "comptype", "Comparison" };
static PRM_Name nameCompValue{ "compvalue", "Value" };
static PRM_Name nameColors{ "colors", "Colors" };
static PRM_Name nameColor{ "color_#", "Color #" };
static PRM_Name nameWeight{ "weight_#", "Weight #" };
static PRM_Name nameBlendMode{ "blendmode", "Blend Mode" };
static PRM_Name nameColorOffset{ "coloroffset", "Color Offset" };
static PRM_Name nameBlendOffset{ "blendoffset", "Blend Offset" };
static PRM_Name nameWeightMult{ "blendweightscale", "Weight Multiplier" };

/// Declare Mode Menu
static PRM_Name menuNameModes[]
{
	PRM_Name("modulus", "Modulus"),
	PRM_Name("comparison", "Comparison"),
	PRM_Name("blendcolors", "Blend Colors"),
	PRM_Name(0)
};

static PRM_Name menuBlendModes[]
{
	PRM_Name("linear", "Linear"),
	PRM_Name("quadratic", "Quadratic"),
	PRM_Name("constant", "Constant"),
	PRM_Name(0)
};

static PRM_ChoiceList menuMode
(
	(PRM_ChoiceListType)(PRM_CHOICELIST_EXCLUSIVE | PRM_CHOICELIST_REPLACE),
	menuNameModes
);

static PRM_ChoiceList menuBlendMode
(
	(PRM_ChoiceListType)(PRM_CHOICELIST_EXCLUSIVE | PRM_CHOICELIST_REPLACE),
	menuBlendModes
);

/// Declare Comparison Menu
static PRM_Name menuNameComparison[]
{
	PRM_Name("less_than", "<"),
	PRM_Name("less_than_equals", "<="),
	PRM_Name("equals", "="),
	PRM_Name("greater_than_equals", ">="),
	PRM_Name("greater_than", ">"),
	PRM_Name("not_equals", "!="),
	PRM_Name(0)
};

static PRM_ChoiceList menuComparison
(
	(PRM_ChoiceListType)(PRM_CHOICELIST_EXCLUSIVE | PRM_CHOICELIST_REPLACE),
	menuNameComparison
);

/// Declare Parm Defaults
static PRM_Default defaultModulo{ 2 };


/// Declare Parm Ranges
static PRM_Range rangeModulo
{
	PRM_RangeFlag::PRM_RANGE_UI, 1,
	PRM_RangeFlag::PRM_RANGE_UI, 10
};

static PRM_Range rangeOffset
{
	PRM_RangeFlag::PRM_RANGE_UI, -10,
	PRM_RangeFlag::PRM_RANGE_UI, 10
};

static PRM_Range rangeWeight
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0.0,
	PRM_RangeFlag::PRM_RANGE_FREE, 10
};

static PRM_Range rangeColors
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 1,
	PRM_RangeFlag::PRM_RANGE_FREE, 10
};

static PRM_Range rangeBlendOffset
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0,
	PRM_RangeFlag::PRM_RANGE_UI, 1
};

static PRM_Range rangeWeightMult
{
	PRM_RangeFlag::PRM_RANGE_UI, 0,
	PRM_RangeFlag::PRM_RANGE_UI, 2
};

static PRM_Template templatesColors[] =
{
	PRM_Template(PRM_RGB, TOOL_PARM, 3, &nameColor, PRMoneDefaults), // White Default
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameWeight, PRMfiveDefaults, 0, &rangeWeight),
	PRM_Template()
};

PRM_Template
COP2_FractalMatte::myTemplateList[] =
{
	PRM_Template(PRM_SWITCHER, 3, &PRMswitcherName, switcher),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameMode, PRMzeroDefaults, &menuMode),
	PRM_Template(PRM_MULTITYPE_LIST, templatesColors, 2, &nameColors, PRMoneDefaults, &rangeColors),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameBlendMode, PRMoneDefaults, &menuBlendMode),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameColorOffset),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameBlendOffset, PRMoneDefaults, 0, &rangeBlendOffset),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameModulo, &defaultModulo, 0, &rangeModulo),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameOffset, PRMzeroDefaults, 0, &rangeOffset),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameCompType, PRMzeroDefaults, &menuComparison),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameCompValue, PRMzeroDefaults),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameWeightMult, PRMoneDefaults, 0, &rangeWeightMult),
	PRM_Template(PRM_TOGGLE_J, TOOL_PARM, 1, &nameInvert, PRMzeroDefaults),
	PRM_Template(),
};

OP_TemplatePair
COP2_FractalMatte::myTemplatePair(
	COP2_FractalMatte::myTemplateList,
	&COP2_PixelOp::myTemplatePair);

OP_VariablePair
COP2_FractalMatte::myVariablePair(0, &COP2_MaskOp::myVariablePair);

const char* COP2_FractalMatte::myInputLabels[] =
{
	"Image to add to",
	"Mask Input",
	0
};

OP_Node* 
COP2_FractalMatte::myConstructor(
	OP_Network* net,
	const char* name,
	OP_Operator* op)
{
	return new COP2_FractalMatte(net, name, op);
}

RU_PixelFunction*
COP2_FractalMatte::addPixelFunction(
	const TIL_Plane *,
	int,
	float t,
	int,
	int,
	int thread)
{
	/// Cast the interface mode option to a ModeType object.
	ModeType mode = 
		(ModeType)evalInt(nameMode.getToken(), 0, t);

	int invert = evalInt(nameInvert.getToken(), 0, t);

	if (mode == ModeType::MODULUS)  // Use the modulus type constructor
	{
		double modulo = evalFloat(nameModulo.getToken(), 0, t);
		double offset = evalFloat(nameOffset.getToken(), 0, t);
		return new cop2_FractalMatteFunc(modulo, offset, invert);
	}
	else if (mode == ModeType::COMPARISON)  // Use the comparison type constructor
	{
		double compValue = evalFloat(nameCompValue.getToken(), 0, t);
		ComparisonType compType = (ComparisonType)evalInt(nameCompType.getToken(), 0, t);
		return new cop2_FractalMatteFunc(compValue, compType, invert);
	}
	else if (mode == ModeType::BLENDCOLOR)  // Use Blend Color Constructor
	{
		std::vector<double>sizes;
		std::vector<UT_Color>colors;

		int numInstances = evalInt(nameColors.getToken(), 0, t);
		for (int i = 0; i < numInstances; ++i)
		{
			int idx = i + 1;
			sizes.emplace_back(evalFloatInst(nameWeight.getToken(), &idx, 0, t));
			UT_Color color
			{
				UT_ColorType::UT_RGB,
				evalFloatInst(nameColor.getToken(), &idx, 0, t),
				evalFloatInst(nameColor.getToken(), &idx, 1, t),
				evalFloatInst(nameColor.getToken(), &idx, 2, t)
			};

			colors.emplace_back(color);
		}

		using BT = cop2_FractalMatteFunc::BlendType;
		BT blendType = (BT)evalInt(nameBlendMode.getToken(), 0, t);

		fpreal32 colorOffset = evalFloat(nameColorOffset.getToken(), 0, t);
		fpreal32 blendOffset = evalFloat(nameBlendOffset.getToken(), 0, t);
		fpreal32 weightMult = evalFloat(nameWeightMult.getToken(), 0, t);
		return new cop2_FractalMatteFunc(
			sizes, colors, blendType, colorOffset, blendOffset, weightMult, invert);
	}
	else
	{
		// Null Fractal Matte that won't respect the interface
		return new cop2_FractalMatteFunc(1.0, 0.0, false);
	}
}

bool COP2_FractalMatte::updateParmsFlags()
{
	// Determine Mode State
	fpreal t = CHgetEvalTime();
	ModeType type = 
		(ModeType)evalInt(nameMode.getToken(), 0, t);

	// Set variables for hiding
	bool displayModulus{ false };
	bool displayComparison{ false };
	bool displayBlendColors{ false };

	if (type == ModeType::COMPARISON)
		displayComparison = true;

	else if (type == ModeType::MODULUS)
		displayModulus = true;

	else if (type == ModeType::BLENDCOLOR)
		displayBlendColors = true;

	// Set the visibility state for hidable parms.
	bool changed = COP2_PixelOp::updateParmsFlags();

	changed |= setVisibleState(nameCompType.getToken(), displayComparison);
	changed |= setVisibleState(nameCompValue.getToken(), displayComparison);
	changed |= setVisibleState(nameModulo.getToken(), displayModulus);
	changed |= setVisibleState(nameOffset.getToken(), displayModulus);
	changed |= setVisibleState(nameColors.getToken(), displayBlendColors);
	changed |= setVisibleState(nameColorOffset.getToken(), displayBlendColors);
	changed |= setVisibleState(nameBlendMode.getToken(), displayBlendColors);
	changed |= setVisibleState(nameBlendOffset.getToken(), displayBlendColors);
	changed |= setVisibleState(nameWeightMult.getToken(), displayBlendColors);
	return changed;
}

COP2_FractalMatte::COP2_FractalMatte(
	OP_Network* net,
	const char* name,
	OP_Operator* op) : COP2_PixelOp(net, name, op) {}

COP2_FractalMatte::~COP2_FractalMatte() {}

cop2_FractalMatteFunc::cop2_FractalMatteFunc(
	double modulo, double offset, bool invert)
{
	this->modulo = modulo;
	this->offset = offset;
	this->invert = invert;
	// Sets the mode type to modulus when this constructor is called.
	this->mode = ModeType::MODULUS;
}

cop2_FractalMatteFunc::cop2_FractalMatteFunc(
	double compValue, ComparisonType compType, bool invert)
{
	this->compValue = compValue;
	this->compType = compType;
	this->invert = invert;
	this->mode = ModeType::COMPARISON;
}

cop2_FractalMatteFunc::cop2_FractalMatteFunc(
	std::vector<double>sizes,
	std::vector<UT_Color>colors,
	BlendType blendType,
	fpreal32 colorOffset,
	fpreal32 blendOffset,
	fpreal32 weightMult,
	bool invert)
{
	// Zero protection of weight multiplier
	fpreal32 scaleMult = 1.0f;
	if (weightMult != 0.0f)
	{
		scaleMult = weightMult;
		// Scale down the color offset relative to the scale mult
		colorOffset *= scaleMult;
	}

	// Assign parameters to object.
	this->blendType = blendType;
	this->mode = ModeType::BLENDCOLOR;
	this->colorOffset = colorOffset;
	this->blendOffset = blendOffset;
	this->invert = invert;

	// Skip colors if their matching size is zero
	std::vector<double> filtered_sizes;
	std::vector<UT_Color> filtered_colors;

	for (int i = 0; i < sizes.size(); ++i)
	{
		// Skip the parameter if zero
		if (sizes[i] != 0.0f)
		{
			filtered_sizes.push_back(sizes[i] * scaleMult);
			filtered_colors.emplace_back(colors[i]);
		}
	}

	this->sizes = filtered_sizes;
	this->colors = filtered_colors;

}

float cop2_FractalMatteFunc::checkModulus(
	RU_PixelFunction * pf,
	float pixelValue,
	int comp)
{
	auto pfCasted = (cop2_FractalMatteFunc*)pf;

	float output = SYSfmod(pixelValue + pfCasted->offset, pfCasted->modulo);
	output = SYSclamp(output, 0.0f, 1.0f);

	// Get the complement of the pixel value if invert is on.
	if (pfCasted->invert)
		output = 1 - output;

	return output;
}

float cop2_FractalMatteFunc::checkComparison(
	RU_PixelFunction * pf, float pixelValue, int comp)
{
	auto pfCasted = (cop2_FractalMatteFunc*)pf;

	fpreal32 output = 0;

	switch (pfCasted->compType)
	{
	case ComparisonType::LESS_THAN:
		output = pixelValue < pfCasted->compValue;
		break;
	case ComparisonType::LESS_THAN_EQUALS:
		output = pixelValue <= pfCasted->compValue;
		break;
	case ComparisonType::EQUALS:
		output = pixelValue == pfCasted->compValue;
		break;
	case ComparisonType::GREATER_THAN_EQUALS:
		output = pixelValue >= pfCasted->compValue;
		break;
	case ComparisonType::GREATER_THAN:
		output = pixelValue > pfCasted->compValue;
		break;
	case ComparisonType::NOT_EQUALS:
		output = pixelValue != pfCasted->compValue;
	default:
		break;
	}

	// Get the complement of the pixel value if invert is on.
	// And in the first channel.
	if (pfCasted->invert && comp == 0)
		output = 1 - output;

	return output;
}

float cop2_FractalMatteFunc::checkBlendColors(
	RU_PixelFunction* pf, float pixelValue, int comp)
{
	auto pfCasted = (cop2_FractalMatteFunc*)pf;

	UT_Color blendColor = pfCasted->colors[0];

	fpreal32 max_val{ 0.0 };
	for (fpreal32 weight : pfCasted->sizes)
		max_val += weight;

	pixelValue = SYSfmod(pixelValue + pfCasted->colorOffset, max_val);

	// Check If exactly equal, set color directly
	for (int i = 0; i < pfCasted->colors.size(); ++i)
	{
		
		if (pixelValue == pfCasted->sizes[i])
		{
			blendColor = pfCasted->colors[i];
			break;
		}
	}

	// Examine each 'range' of values, and attempt to calculate a blendcolor
	float low = 0.0f, high =0.0f;
	for (int i = 0; i < pfCasted->colors.size(); ++i)
	{
		// Loops back to 0 on last entry
		fpreal size = pfCasted->sizes[i];
		high = low + size;

		// Check if in range
		if (pixelValue > low && pixelValue < high)
		{
			float weight = SYSfit(pixelValue, low, high, 0.0f, 1.0f);
			blendColor = pfCasted->colors[i];
			int nextColorIdx = (i + 1) % pfCasted->colors.size();
			// Blend the colors. Linear Blendtype will not be blended.
			bool doBlend{ false };
			if (pfCasted->blendType == BlendType::LINEAR)
				doBlend = true;
			else if (pfCasted->blendType == BlendType::QUADRATIC)
			{
				doBlend = true;
				weight = SYSsqrt(weight);
			}

			// Get complement of weight when inverted
			if (pfCasted->invert)
				weight = 1.0f - weight;

			// Blend
			if (doBlend)
			{
				// Offset the max range of the weight.
				if (pfCasted->blendOffset != 0.0)
					weight = SYSfit(weight, 0.0f, pfCasted->blendOffset, 0.0f, 1.0f);
				blendColor.blendRGB(pfCasted->colors[nextColorIdx], weight);
			}
			break;
		}
		low = high;
	}

	// Filter by plane.
	float val;
	UT_Vector3F rgb{ blendColor.rgb() };
	if (comp == 0)
		val = rgb.r();
	else if (comp == 1)
		val = rgb.g();
	else if (comp == 2)
		val = rgb.b();

	return val;

}