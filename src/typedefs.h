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
static const char* ITERS_NAME{ "iters" };  // iterations