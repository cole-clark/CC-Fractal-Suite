/*
	Cole Clark's Fractal Suite

	COP2_FractalMatte.h
	Code for CC Fractal Matte Cop filter node.
 */

#include <PRM/PRM_Include.h>
#include <CH/CH_Manager.h>

#include "COP2_FractalMatte.h"

using namespace CC;

typedef cop2_FractalMatteFunc::ModeType ModeType;
typedef cop2_FractalMatteFunc::ComparisonType ComparisonType;

COP_PIXEL_OP_SWITCHER(6, "Mattes");

/// Declare Parm Names
static PRM_Name nameModulo{ "modulo", "Modulo" };
static PRM_Name nameOffset{ "offset", "Offset" };
static PRM_Name nameInvert{ "invert", "Invert" };
static PRM_Name nameMode{ "mode", "Mode" };
static PRM_Name nameCompType{ "comptype", "Comparison" };
static PRM_Name nameCompValue{ "compvalue", "Value" };

/// Declare Mode Menu
static PRM_Name menuNameModes[]
{
	PRM_Name("modulus", "Modulus"),
	PRM_Name("comparison", "Comparison"),
	PRM_Name(0)
};

static PRM_ChoiceList menuMode
(
	(PRM_ChoiceListType)(PRM_CHOICELIST_EXCLUSIVE | PRM_CHOICELIST_REPLACE),
	menuNameModes
);

/// Declare Comparison Menu
static PRM_Name menuNameComparison[]
{
	PRM_Name("less_than", "<"),
	PRM_Name("less_than_equals", "<="),
	PRM_Name("equals", "="),
	PRM_Name("greater_than_equals", ">="),
	PRM_Name("greater_than", ">"),
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

PRM_Template
COP2_FractalMatte::myTemplateList[] =
{
	PRM_Template(PRM_SWITCHER, 3, &PRMswitcherName, switcher),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameMode, PRMzeroDefaults, &menuMode, 0, helpMode),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameModulo, &defaultModulo, 0, &rangeModulo),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameOffset, PRMzeroDefaults, 0, &rangeOffset),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameCompType, PRMzeroDefaults, &menuComparison),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameCompValue, PRMzeroDefaults),
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
	else  // Use the comparison type constructor
	{
		double compValue = evalFloat(nameCompValue.getToken(), 0, t);
		ComparisonType compType = (ComparisonType)evalInt(nameCompType.getToken(), 0, t);
		return new cop2_FractalMatteFunc(compValue, compType, invert);
	}
}

bool COP2_FractalMatte::updateParmsFlags()
{
	// Determine Mode State
	fpreal t = CHgetEvalTime();
	ModeType type = 
		(ModeType)evalInt(nameMode.getToken(), 0, t);

	// Set variables for hiding
	bool displayModulus{ true };
	bool displayComparison{ false };

	if (type == ModeType::COMPARISON)
	{
		displayModulus = false;
		displayComparison = true;
	}

	// Set the visibility state for hidable parms.
	bool changed = COP2_PixelOp::updateParmsFlags();

	changed |= setVisibleState(nameCompType.getToken(), displayComparison);
	changed |= setVisibleState(nameCompValue.getToken(), displayComparison);
	changed |= setVisibleState(nameModulo.getToken(), displayModulus);
	changed |= setVisibleState(nameOffset.getToken(), displayModulus);

	return changed;
}

COP2_FractalMatte::COP2_FractalMatte(
	OP_Network* net,
	const char* name,
	OP_Operator* op) : COP2_PixelOp(net, name, op) {}

COP2_FractalMatte::~COP2_FractalMatte() {}

const char*
COP2_FractalMatte::getInfoPopup()
{
	fpreal t = CHgetEvalTime();

	static UT_WorkBuffer info;
	info.sprintf("Generating fractal mattes in chunks of %g",
		evalInt(nameModulo.getToken(), 0, t));
	return nullptr;
}

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

	float output = 0;

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
	default:
		break;
	}

	// Get the complement of the pixel value if invert is on.
	if (pfCasted->invert)
		output = 1 - output;

	return output;
}


