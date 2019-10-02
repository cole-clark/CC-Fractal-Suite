/** \file Fractal.h
	Header-only include that defines base Fractal objects

 */

#pragma once

 // Local
#include "typedefs.h"
#include "FractalSpace.h"

namespace CC
{

/**A small container struct for fractal output.
 * Fractals can return one or more of these types, it is up to the
 * Fractal author to decide which ones actually get used. Mandelbrots for
 * example can sensibly return all these values, while Pickovers don't have
 * a meaningful 'num_iter' in their equation.
 */
struct FractalCoordsInfo
{
	int num_iter{ 0 };
	COMPLEX z;
	fpreal smooth;

	FractalCoordsInfo(
		int num_iter = 0,
		COMPLEX z = COMPLEX(),
		fpreal smooth = 0.0) :
		num_iter(num_iter), z(z), smooth(smooth) {}
};

/**Base class for fractals. */
class Fractal
{
public:
	Fractal() {};
	virtual ~Fractal() {};

	/** Returns Fractal Info for a particular set of coords.
	 * This is the only 'legal' way to calculate a fractal. This
	 * is because while a Fractal Node may have different cooking
	 * schemes, this enforces a uniformity with mathematically how
	 * the fractals cook. In other words, any COP2 node cooking type
	 * can theoretically cook any Fractal.
	 */
	virtual FractalCoordsInfo calculate(COMPLEX coords) = 0;
};
} // End of CC Namespace