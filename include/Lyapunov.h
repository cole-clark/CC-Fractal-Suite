/** \file Lyapunov.h
	Header declaring the Lyapunov fractal class.
 */

#pragma once

 // Local
#include "Fractal.h"
#include "FractalSpace.h"

namespace CC
{
/**Lyapunov Fractal Class. Lyapunovs are fundamentally
 * distinct from the other Mandelbrot-based fractals in the
 * CCFS. They are not self similar, and are optically
 * fairly primitive. However, they don't require deep zooms,
 * require few iterations to achieve a 'final' looking quality,
 * and have many different ways of being animated.*/
class Lyapunov : public Fractal
{
public:
	LyapunovStashData data;

	Lyapunov() = default;

	/**Construct a Lyapunov Fractal with data from a
	 * LyapunovStashData.*/
	Lyapunov(LyapunovStashData& lyaData);
	virtual ~Lyapunov();

	FractalCoordsInfo calculate(COMPLEX coords);

	/**Returns a sequence of values based on data.seq.
	 * When seq is equal to zero, 'X' is returned, and when seq is equal to one
	 * 'Y' returned. All other values are linearly interpretpreted between 'X'
	 * and 'Y'. 'X' and 'Y' symbolize cartesian coordinates. Lerping these
	 * values has the effect of 'rotating' a spike in the Lyapunov image.
	 */
	std::vector<fpreal> generate_sequence(fpreal x, fpreal y);
};
}