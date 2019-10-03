/** \file FractalNode.h
	Header-only include that generically declares a ton of
	static variables and Parm Template Macros for parameters to be used
	all across the CCFS.

 */

#pragma once

 // Local
#include "typedefs.h"

// STL
#include <string>
#include <sstream>
#include <iomanip>

// HDK
#include <PRM/PRM_Include.h>
#include <PRM/PRM_ChoiceList.h>
#include <OP/OP_Network.h>
#include <CH/CH_Manager.h>
#include <COP2/COP2_Common.h>


// Fractal Xform Name Data
static PRM_Name nameScale(SCALE_NAME.first, SCALE_NAME.second);
static PRM_Name nameOffset(TRANSLATE_NAME.first, TRANSLATE_NAME.second);
static PRM_Name nameRotate(ROTATE_NAME.first, ROTATE_NAME.second);
static PRM_Name nameXOrd(XORD_NAME.first, XORD_NAME.second);

// Fractal Xform Multi Name Data
static PRM_Name nameXforms{ XFORMS_NAME.first, XFORMS_NAME.second };
static PRM_Name nameMultiScale(SCALE_M_NAME.first, SCALE_M_NAME.second);

static PRM_Name nameMultiOffset(
	TRANSLATE_M_NAME.first,
	TRANSLATE_M_NAME.second);

static PRM_Name nameMultiRotate(ROTATE_M_NAME.first, ROTATE_M_NAME.second);
static PRM_Name nameMultiXOrd(XORD_M_NAME.first, XORD_M_NAME.second);

// Mandelbrot Name Data
static PRM_Name nameIter{ ITERS_NAME.first, ITERS_NAME.second };
static PRM_Name namePower{ POWER_NAME.first, POWER_NAME.second };
static PRM_Name nameBailout{ BAILOUT_NAME.first, BAILOUT_NAME.second };
static PRM_Name nameJDepth{ JDEPTH_NAME.first, JDEPTH_NAME.second };
static PRM_Name nameJOffset{ JOFFSET_NAME.first, JOFFSET_NAME.second };
static PRM_Name nameBlackhole{ BLACKHOLE_NAME.first, BLACKHOLE_NAME.second };

// Pickover Name Data
static PRM_Name namePoPoint(
	POPOINT_NAME.first,
	POPOINT_NAME.second);

static PRM_Name namePoMode(
	POMODE_NAME.first,
	POMODE_NAME.second);

static PRM_Name namePoLineRotate(
	POROTATE_NAME.first,
	POROTATE_NAME.second);

static PRM_Name namePoReference(
	POREFERENCE_NAME.first,
	POREFERENCE_NAME.second);

static PRM_Name namePoRefSize(
	POREFSIZE_NAME.first,
	POREFSIZE_NAME.second);

// Declare Lyapunov Parm Names
static PRM_Name nameLyaSeq(
	LYASEQ_NAME.first,
	LYASEQ_NAME.second);

static PRM_Name nameLyaSeqWeights(
	LYASEQWEIGHTS_NAME.first,
	LYASEQWEIGHTS_NAME.second);

static PRM_Name nameLyaStart(
	LYASTART_NAME.first,
	LYASTART_NAME.second);

static PRM_Name nameLyaMaxValue(
	LYACEILVALUE_NAME.first,
	LYACEILVALUE_NAME.second);

static PRM_Name nameLyaInvertNegative(
	LYAINVERTNEGATIVE_NAME.first,
	LYAINVERTNEGATIVE_NAME.second);


// ChoiceList Lists
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

/// ChoiceList Lists
static PRM_Name poModeMenuNames[] =
{
	PRM_Name("point", "Point"),
	PRM_Name("line", "Line"),
	PRM_Name(0)
};

static PRM_ChoiceList poModeMenu
(
(PRM_ChoiceListType)(PRM_CHOICELIST_EXCLUSIVE | PRM_CHOICELIST_REPLACE),
::poModeMenuNames
);

// Xform Defaults Data
/** These values are chosen to look nice for a default Mandelbrot. */
static PRM_Default defaultScale{ 5 };
/** The Offset Y value is equal to 1080 / 1920 * -0.5 */
static PRM_Default defaultOffset[] = { -0.6, -0.28125 };
static PRM_Default defaultRotate{ 0.0f };
static PRM_Default defaultXOrd{ RSTORDER::TRS };

/** This value is chosen to work better with multiple xforms*/
static PRM_Default defaultMultiXOrd{ RSTORDER::RST };

/** These values are chosen to look nice for a default Buddhabrot, which have
 * a different default transformation order.
 */
static PRM_Default defaultOffsetBuddhabrot[] = { -3.0, -1.3 };

// Mandelbrot Defaults Data
static PRM_Default defaultIter{ 50 };
static PRM_Default defaultPower{ 2 };

/** Canonically this should be 2, but 4 Looks better at 4k when smoothing. */
static PRM_Default defaultBailout{ 4 };
static PRM_Default defaultJDepth{ 0 };
static PRM_Default defaultJOffset[] = { 0, 0 };
static PRM_Default defaultBlackhole{ false };

// Define Pickover Defaults
static PRM_Default defaultPoRefSize{ 10.0 };

// Declare Lyapunov Defaults
static PRM_Default defaultLyaMaxValue(5.0f);

/**Canonically, lyapunovs start at 0.5, but the CCFS exposed this as a parm.*/
static PRM_Default defaultLyaStart(0.5);

// Xform Parm Ranges
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

// Mandelbrot Parm Ranges
static PRM_Range rangeIter
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 1,
	PRM_RangeFlag::PRM_RANGE_UI, 200
};


static PRM_Range rangePower
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

// Lyapunov Ranges

static PRM_Range rangeLyaStartValue
{
	PRM_RangeFlag::PRM_RANGE_UI, 0.001f,
	PRM_RangeFlag::PRM_RANGE_UI, 0.999f
};

static PRM_Range rangeLyaMaxValue
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 1,
	PRM_RangeFlag::PRM_RANGE_UI, defaultLyaMaxValue.getFloat()
};

static PRM_Range rangeLyaSeq
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 1,
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 40
};

static PRM_Range rangeLyaIters
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 1,
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 40
};

// Define Pickover ranges
static PRM_Range rangePoRotate
{
	PRM_RangeFlag::PRM_RANGE_UI, -180,
	PRM_RangeFlag::PRM_RANGE_UI, 180
};

static PRM_Range rangePoRefSize
{
	PRM_RangeFlag::PRM_RANGE_RESTRICTED, 0.0,
	PRM_RangeFlag::PRM_RANGE_FREE, 25
};

// Multiparm Templates
static PRM_Template multiparmSeqTemps[] =
{
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, &nameLyaSeqWeights, PRMzeroDefaults),
	PRM_Template()
};

static PRM_Template multiparmXformTemps[] =
{
	PRM_Template(PRM_INT_J, TOOL_PARM, 1,
		&nameMultiXOrd, &defaultMultiXOrd, &xOrdMenu),
	PRM_Template(PRM_FLT_LOG, TOOL_PARM, 1,
		&nameMultiScale, PRMoneDefaults, 0, &rangeScale),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2,
		&nameMultiOffset, PRMzeroDefaults),
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1,
		&nameMultiRotate, PRMzeroDefaults, 0, &rangeRotate),
	PRM_Template()
};

// Create separator names. These are shared across the CCFS
static PRM_Name nameSeparatorMandelbrot("sep_mandelbrot", "Sep Mandelbrot");
static PRM_Name nameSepA("sep_A", "Sep A");
static PRM_Name nameSepB("sep_B", "Sep B");
static PRM_Name nameSepC("sep_C", "Sep C");

// Macro for creating the Switcher
#define TEMPLATE_SWITCHER PRM_Template(PRM_SWITCHER, 4, \
	&PRMswitcherName, switcher)

/** Macro for creating Xform Templates for myTemplateList calls.
 * Add 4 to COP_SWITCHER calls. */
#define TEMPLATES_XFORM \
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, \
		&nameXOrd, &defaultXOrd, &xOrdMenu), \
	PRM_Template(PRM_FLT_LOG, TOOL_PARM, 1, \
		&nameScale, &defaultScale, 0, &rangeScale), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, \
		&nameOffset, defaultOffset), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, \
		&nameRotate, PRMzeroDefaults, 0, &rangeRotate)

 /** Macro for creating Multi-Xform parameters
  * Add 1 to COP_SWITCHER calls. */
#define TEMPLATES_XFORM_MULTI \
	PRM_Template(PRM_MULTITYPE_LIST, multiparmXformTemps, 1, \
		&nameXforms, PRMzeroDefaults)

  /** Macro for creating the Xform Parameters, which have removed the Xord
   *  option and set more appropriate defaults for Buddhabrots. */
#define TEMPLATES_XFORM_BUDDHABROT \
	PRM_Template(PRM_FLT_LOG, TOOL_PARM, 1, \
		&nameScale, &defaultScale, 0, &rangeScale), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, \
		&nameOffset, defaultOffsetBuddhabrot), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, \
		&nameRotate, PRMzeroDefaults, 0, &rangeRotate)

   /** Macro for creating Mandelbrot Templates.
	* Add 7 to COP_SWITCHER calls
	*/
#define TEMPLATES_MANDELBROT \
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, \
		&nameIter, &defaultIter, 0, &rangeIter), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, \
		&namePower, &defaultPower, 0, &rangePower), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, \
		&nameBailout, &defaultBailout, 0, &rangeBailout), \
	PRM_Template(PRM_TOGGLE_J, TOOL_PARM, 1, \
		&nameBlackhole, PRMzeroDefaults), \
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, \
		&nameSeparatorMandelbrot, PRMzeroDefaults), \
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, \
		&nameJDepth, PRMzeroDefaults, 0, &rangeJDepth), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, \
		&nameJOffset, PRMzeroDefaults)

	/** Macro for creating Pickover Templates.
	 * Add 6 to COP_SWITCHER calls.
	 * Pickovers are dependent on TEMPLATES_MANDELBROT also being declared
	 * in the template argument array.
	 */
#define TEMPLATES_PICKOVER \
	PRM_Template(PRM_SEPARATOR, TOOL_PARM, 1, &nameSepB), \
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, \
		&namePoMode, PRMoneDefaults, &poModeMenu), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, \
		&namePoPoint, PRMzeroDefaults), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, \
		&namePoLineRotate, PRMzeroDefaults, 0, &rangePoRotate), \
	PRM_Template(PRM_TOGGLE_J, TOOL_PARM, 1, \
		&namePoReference, PRMoneDefaults), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, \
		&namePoRefSize, &defaultPoRefSize, 0, &rangePoRefSize)


	 /** Macro for creating Lyapunov Templates.
	  * Add 5 to COP_SWITCHER calls.
	 */
#define TEMPLATES_LYAPUNOV \
	PRM_Template(PRM_INT_J, TOOL_PARM, 1, \
		&nameIter, &defaultIter, 0, &rangeLyaIters), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, \
		&nameLyaStart, &defaultLyaStart, 0, &rangeLyaStartValue), \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 1, \
		&nameLyaMaxValue, &defaultLyaMaxValue, 0, &rangeLyaMaxValue), \
	PRM_Template(PRM_TOGGLE_J, TOOL_PARM, 1, \
		&nameLyaInvertNegative, PRMoneDefaults), \
	PRM_Template(PRM_MULTITYPE_LIST, multiparmSeqTemps, 1, \
		&nameLyaSeq, PRMoneDefaults, &rangeLyaSeq)


namespace CC
{
/** OP is meant to remove the need to write public 'myConstructor'
 * methods in every class like the examples. So long as the constructor for
 * a node can be default, this class need only be added as a 'friend class'
 * and register.cpp will be able to call the static get_new_op method to
 * pass node construction to Houdini.
 */
class OP
{
public:
	/**Method that calls a node's constructor, if added as a friend of
	 * the given node's class. See register.cpp to see its usage.*/
	template <typename T>
	static OP_Node* get_new_op(
		OP_Network* net,
		const char* name,
		OP_Operator* op)
	{
		return new T(net, name, op);
	}
};

/**A simple container struct that represents the total output of a
 * MultiXform transformation.
 */
struct MultiXformData
{
	fpreal scale{ 1.0 };
	COMPLEX offset{ 0.0, 0.0 };
	fpreal rotate{ 0.0 };
};


/** Takes a node with TEMPLATES_XFORM_MULTI defined, and returns
 * The overall requested offset and scale.
 */
static MultiXformData
get_multi_xform_sums(OP_Node* node)
{
	MultiXformData data;

	fpreal t = CHgetEvalTime();

	int numXforms = node->evalInt(XFORMS_NAME.first, 0, t);
	for (int i = 1; i <= numXforms; ++i)
	{
		data.scale *= node->evalFloatInst(
			SCALE_M_NAME.first, &i, 0, t);

		data.offset += (
			node->evalFloatInst(TRANSLATE_M_NAME.first, &i, 0, t),
			node->evalFloatInst(TRANSLATE_M_NAME.first, &i, 1, t));

		data.rotate += node->evalFloatInst(
			ROTATE_M_NAME.first, &i, 0, t);
	}

	return data;
};

/**Formats the information from a MultiXformData object into something
 * that is nice to print. */
static std::string
formatMultiXformData(MultiXformData data)
{
	std::ostringstream oss;

	oss << std::setprecision(2);
	oss << "Fractal Transformation:" << std::endl;
	oss << "Scale: " << data.scale << std::endl;
	oss << "Transform: " << data.offset.real() << " "
		<< data.offset.imag() << std::endl;
	oss << "Rotate: " << data.rotate << std::endl;

	return oss.str();
}
} // End of CC Namespace
