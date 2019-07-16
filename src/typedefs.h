/*
	Cole Clark's Fractal Suite

	typedefs.h
	Header defining typedefs used by the fractal suite.
	Created here to avoid cyclical imports
 */

#pragma once

#include <complex>

#include <UT/UT_XformOrder.h>

typedef std::complex<double> COMPLEX;          /// For Complex Numbers
typedef std::pair<int, int> WORLDPIXELCOORDS;  /// For the pixel coordinates of the image
typedef UT_XformOrder::rstOrder RSTORDER;      /// To shorten the RSTOrder of a Matrix

/// Definitions for generically referring to parameter names
static const char* TRANSLATE_NAME{ "offset" };
static const char* ROTATE_NAME{ "rotate" };
static const char* SCALE_NAME{ "scale" };
static const char* XORD_NAME{ "xOrd" };