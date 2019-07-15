/*
	Cole Clark's Fractal Suite

	FractalNode.h
	Header defining shared fundamentals of Fractal Nodes
 */

#pragma once

#include <string>

#include <PRM/PRM_Include.h>
#include <PRM/PRM_ChoiceList.h>
#include <OP/OP_Network.h>

/// Fractal Xform Data

static PRM_Name nameScale("scale", "Scale");
static PRM_Name nameOffset("offset", "Offset");
static PRM_Name nameRotate("rotate", "Rotate");
static PRM_Name nameXOrd("xOrd", "Xform Order");

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

static PRM_Default defaultScale{ 1 };
static PRM_Default defaultOffset{ 0, 0 };
static PRM_Default defaultRotate{ 0 };
static PRM_Default defaultXOrd{ 0 };  // TRS

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

/// Definition of Template Switcher
#define TEMPLATE_SWITCHER PRM_Template(PRM_SWITCHER, 4, &PRMswitcherName, switcher)

/// Definition of Xform Templates for myTemplateList calls, add 4 to COP_SWITCHER calls
#define TEMPLATES_XFORM \
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, &nameXOrd, &defaultXOrd, &xOrdMenu), \
	PRM_Template(PRM_FLT_LOG, TOOL_PARM, 1, &nameScale, &defaultScale, 0, &rangeScale), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameOffset, &defaultOffset), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameRotate, PRMzeroDefaults, 0, &rangeRotate)


/// Parm separator factory code. Call MAKE_SEP_NAME for each separator needed,
/// And then call MAKE_SEP_TEMPLATE with the same index when building the template arg list.
/// Be sure to not have matching indexes within the same node.
#define MAKE_SEP_NAME(i) \
	static PRM_Name nameSeparator##i( \
		static_cast<UT_StringHolder>("sep" + std::to_string(##i)), \
		static_cast<UT_StringHolder>("sep" + std::to_string(##i)))

#define MAKE_SEP_TEMPLATE(i) \
		PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSeparator##i)

MAKE_SEP_NAME(1);
MAKE_SEP_NAME(2);
MAKE_SEP_NAME(3);
MAKE_SEP_NAME(4);
MAKE_SEP_NAME(5);
MAKE_SEP_NAME(6);
MAKE_SEP_NAME(7);
MAKE_SEP_NAME(8);
MAKE_SEP_NAME(9);
MAKE_SEP_NAME(10);

/// Boilerplate code calling the constructor for newCop2Operator.
/// Do not use this if you need to modify the class constructor.
/// This is only meant for vanilla calls.
namespace CC
{
	static class OP
	{
	public:
		template <typename T>
		static OP_Node* get_new_op(
			OP_Network* net,
			const char* name,
			OP_Operator* op)
		{
			return new T(net, name, op);
		}
	};
}