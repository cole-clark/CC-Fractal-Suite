/*
	Cole Clark's Fractal Suite

	typedefs.h
	Header defining typedefs used by the fractal suite.
	Created here to avoid cyclical imports
 */

#pragma once

#include <complex>
#include <utility>
#include <UT/UT_XformOrder.h>

typedef std::complex<double> COMPLEX;          /// For Complex Numbers
typedef std::pair<int, int> WORLDPIXELCOORDS;  /// For the pixel coordinates of the image
typedef UT_XformOrder::rstOrder RSTORDER;      /// To shorten the RSTOrder of a Matrix
typedef std::pair<const char*, const char*> NAMEPAIR; /// To shorten names
/// Definitions for generically referring to parameter names
/// Xform Names

static NAMEPAIR TRANSLATE_NAME{ "offset", "Offset" };
static NAMEPAIR ROTATE_NAME{ "rotate", "Rotate" };
static NAMEPAIR SCALE_NAME{ "scale", "Scale" };
static NAMEPAIR XORD_NAME{ "xOrd", "TRS Order"};

/// Mandelbrot Names
static NAMEPAIR ITERS_NAME{ "iters", "Iterations" };
static NAMEPAIR POWER_NAME{ "pow", "Exponent" };
static NAMEPAIR BAILOUT_NAME{ "bailout", "Bailout" };
static NAMEPAIR JDEPTH_NAME{ "jdepth", "Julia Depth" };
static NAMEPAIR JOFFSET_NAME{ "joffset", "Julia Offset" };
static NAMEPAIR BLACKHOLE_NAME{ "blackhole", "Blackhole" };

/// Pickover names
static NAMEPAIR POPOINT_NAME{ "popoint", "Pickover Point" };
static NAMEPAIR POROTATE_NAME{ "porotate", "Pickover Rotate" };
static NAMEPAIR POMODE_NAME{ "pomode", "Pickover Mode" };