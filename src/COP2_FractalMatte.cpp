/*
	Cole Clark's Fractal Suite

	COP2_FractalMatte.h
	Code for CC Fractal Matte Cop filter node.
 */

#include <PRM/PRM_Include.h>
#include <CH/CH_Manager.h>

#include "COP2_FractalMatte.h"

using namespace CC;

COP_PIXEL_OP_SWITCHER(1, "Mattes");

/// Declare Parm Names
static PRM_Name nameModulo{ "modulo", "Modulo" };
static PRM_Name nameOffset{ "offset", "Offset" };

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
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameModulo, &defaultModulo, 0, &rangeModulo),
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameOffset, PRMzeroDefaults, 0, &rangeOffset),
	PRM_Template()
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
	int modulo = evalInt(nameModulo.getToken(), 0, t);
	int offset = evalInt(nameOffset.getToken(), 0, t);
	return new cop2_FractalMatteFunc(modulo, offset);
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

cop2_FractalMatteFunc::cop2_FractalMatteFunc(int modulo, int offset)
{
	this->modulo = modulo;
	this->offset = offset;
}

float cop2_FractalMatteFunc::checkModulus(
	RU_PixelFunction * pf,
	float pixelValue,
	int comp)
{
	auto pfCasted = (cop2_FractalMatteFunc*)pf;
	return SYSfmod(pixelValue + pfCasted->offset, pfCasted->modulo);
}


