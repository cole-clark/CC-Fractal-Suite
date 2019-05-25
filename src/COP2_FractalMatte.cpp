/*
	Cole Clark's Fractal Suite

	COP2_Mandelbrot.cpp
	Code for CC Fractal Matte Filter Cop Node.
 */

#include "COP2_FractalMatte.h"

#include <COP2/COP2_CookAreaInfo.h>
#include <PRM/PRM_Include.h>

using namespace CC;

COP_MASK_SWITCHER(1, "Mattes");

static PRM_Name nameModulo ("modulo", "Modulo");

PRM_Template
COP2_FractalMatte::myTemplateList[] =
{
	PRM_Template(PRM_SWITCHER, 3, &PRMswitcherName, switcher),
	PRM_Template(PRM_INT_J, 1, &nameModulo, PRMzeroDefaults),
	PRM_Template()
};

OP_TemplatePair COP2_FractalMatte::myTemplatePair(COP2_FractalMatte::myTemplateList,
	&COP2_MaskOp::myTemplatePair);

OP_VariablePair COP2_FractalMatte::myVariablePair(0, &COP2_Node::myVariablePair);

const char* COP2_FractalMatte::myInputLabels[] =
{
	"Fractal",
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

COP2_FractalMatte::COP2_FractalMatte(
	OP_Network* net,
	const char* name,
	OP_Operator* op) : COP2_MaskOp(net, name, op) {}


COP2_FractalMatte::~COP2_FractalMatte() {}

COP2_ContextData*
COP2_FractalMatte::newContextData(
	const TIL_Plane*,
	int,
	float t,
	int xres,
	int yres,
	int,
	int)
{
	COP2_FractalMatteData* data = new COP2_FractalMatteData();

	return data;
}

/// Expand the image bounds by one.
void
COP2_FractalMatte::computeImageBounds(COP2_Context& context)
{
	int x1, x2, y1, y2;
	// Grab the bounds of the mask op, which combines mask with image bounds.
	COP2_MaskOp::computeImageBounds(context);
	// Enlarge by 1.
	context.getImageBounds(x1, y1, x2, y2);
	context.setImageBounds(x1 - 1, y1 - 1, x2 + 1, y2 + 1);
}

/// Tell the scheduler what parts of the input's image data is required.
void
COP2_FractalMatte::getInputDependenciesForOutputArea(
	COP2_CookAreaInfo& output_area,
	const COP2_CookAreaList& input_areas,
	COP2_CookAreaList& needed_areas)
{
	// Add dependencies on the first input and the mask plane.
	COP2_MaskOp::getInputDependenciesForOutputArea(
		output_area, input_areas, needed_areas);

	// If bypassed, do nothing.
	if (getBypass())
		return;

	// Enlarge the needed area of the first input by 1 pixel in all directions.
	COP2_CookAreaInfo* inarea =
		makeOutputAreaDependOnMyPlane(0, output_area, input_areas, needed_areas);

	// May not exist if the input node has an error.
	if (inarea)
		inarea->expandNeededArea(1, 1, 1, 1);
}

/// Friendly node-info.
const char*
COP2_FractalMatte::getOperationInfo()
{
	return "This operation returns mattes from a fractal's integer output.";
}

OP_ERROR
COP2_FractalMatte::doCookMyTile(COP2_Context& context, TIL_TileList* tiles)
{
	// Grab our context data.
	COP2_FractalMatteData* data = static_cast<COP2_FractalMatteData *>(context.data());

	TIL_Region* in = inputRegion(0, context,
		tiles->myX1 - 1,
		tiles->myY1 - 1,
		tiles->myX2 + 1,
		tiles->myY2 + 1,
		TIL_HOLD);

	if (!in)
	{
		tiles->clearToBlack();
		return error();
	}

	// Call the operation

	return error();
}

CC::COP2_FractalMatteData::COP2_FractalMatteData()
{
}

CC::COP2_FractalMatteData::~COP2_FractalMatteData()
{
}
