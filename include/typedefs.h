/** \file typedefs.h
	Header with shared typedefs and parameter name declarations.

 * NAMEPAIRs define one of the biggest perks of the CCFS--the ability
 * to manage groups of parameters, and their evaluation generically rather than
 * per-node. The namepairs in this file are used ubiquitously in both the
 * FractalNode.h static declaration of the parameter names, along with the
 * related stash data objects that actually cook the nodes based on those
 * parameter names. To rename the true name of a parameter (dangerous), rename
 * the first c-string in the NAMEPAIR, and to rename the nice name of a
 * parameter (safe), rename the second c-string in the NAMEPAIR.
 */

#pragma once

 // STL
#include <complex>
#include <utility>

// HDK
#include <UT/UT_XformOrder.h>
#include <SYS/SYS_Types.h>

/** Double precision complex numbers, as defined by the HDK. */
typedef std::complex<fpreal64> COMPLEX;
/** Integer pixel coordinate data type. */
typedef std::pair<int, int> WORLDPIXELCOORDS;
/** Short definition of Houdini's Transform Order enum used by matrices. */
typedef UT_XformOrder::rstOrder RSTORDER;
/** Short definition of two c-strings that correspond to Houdini parm names. */
typedef std::pair<const char*, const char*> NAMEPAIR;


/** Xform translate parm name */
static NAMEPAIR TRANSLATE_NAME{ "translate", "Translate" };

/** Xform rotate parm name */
static NAMEPAIR ROTATE_NAME{ "rotate", "Rotate" };

/** Xform uniform scale parm name */
static NAMEPAIR SCALE_NAME{ "scale", "Scale" };

/** Xform transformation order parm name */
static NAMEPAIR XORD_NAME{ "xOrd", "TRS Order" };

/** Number of transforms for a multi-transform fractal space multiparm name */
static NAMEPAIR XFORMS_NAME{ "xforms", "Transforms" };

/** Multi-Xform translation multiparm name */
static NAMEPAIR TRANSLATE_M_NAME{ "tranlate_#", "Translate #" };

/** Multi-Xform rotation multiparm name */
static NAMEPAIR ROTATE_M_NAME{ "rotate_#", "Rotate #" };

/** Multi-Xform uniform scale multiparm name */
static NAMEPAIR SCALE_M_NAME{ "scale_#", "Scale #" };

/** Multi-Xform transformation order multiparm name */
static NAMEPAIR XORD_M_NAME{ "xOrd_#", "TRS Order #" };

/** Mandelbrot Fractal Number of Iterations parm name */
static NAMEPAIR ITERS_NAME{ "iters", "Iterations" };

/** Mandelbrot Fractal exponent applied to fractal z parm name */
static NAMEPAIR POWER_NAME{ "pow", "Exponent" };

/** Mandelbrot Fractal bailout or 'escape' parm name */
static NAMEPAIR BAILOUT_NAME{ "bailout", "Bailout" };

/** Mandelbrot Fractal Number of Julia sets parm name */
static NAMEPAIR JDEPTH_NAME{ "jdepth", "Julia Depth" };

/** Mandelbrot Fractal Julia set position offset parm name */
static NAMEPAIR JOFFSET_NAME{ "joffset", "Julia Offset" };

/** Mandelbrot Fractal make escaped values black parm name */
static NAMEPAIR BLACKHOLE_NAME{ "blackhole", "Blackhole" };

/** Pickover Fractal point position and line offset parm name */
static NAMEPAIR POPOINT_NAME{ "popoint", "Pickover Point" };

/** Pickover Fractal rotate line parm name */
static NAMEPAIR POROTATE_NAME{ "porotate", "Pickover Rotate" };

/** Pickover Fractal mode choice parm name */
static NAMEPAIR POMODE_NAME{ "pomode", "Pickover Mode" };

/** Pickover Fractal display reference image parm name */
static NAMEPAIR POREFERENCE_NAME{ "poreference", "Display Reference" };

/** Pickover Fractal size of reference image line or dot parm name */
static NAMEPAIR POREFSIZE_NAME{ "porefsize", "Reference Size" };

/** Lyapunov Fractal number of 'X' or 'Y' axis selections parm names */
static NAMEPAIR LYASEQ_NAME{ "seq", "Sequence" };

/** Lyapunov Fractal weight where X = 0 and Y = 1 parm names */
static NAMEPAIR LYASEQWEIGHTS_NAME{ "weight_#", "Weight #" };

/** Lyapunov Fractal value of automatic first item in sequence parm name */
static NAMEPAIR LYASTART_NAME{ "seqstart", "Seq Start Value" };

/** Lyapunov Fractal highest value returned parm name */
static NAMEPAIR LYACEILVALUE_NAME{ "ceilval", "Ceiling Value" };

/** Lyapunov Fractal make negative values positive parm name */
static NAMEPAIR LYAINVERTNEGATIVE_NAME{ "invertnegative", "Invert Negative" };
