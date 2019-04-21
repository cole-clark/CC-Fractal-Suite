#pragma once

#include <UT/UT_Vector2.h>

#include "Fractal.h"
#include "FractalSpace.h"

namespace CC
{

	/// Declare the mandelbrot to be used
	class Mandelbrot : public Fractal
	{
		int iter{ 0 };
		float pow{ 2 };
		float bailout{ 2 };
		int jdepth{ 0 };  // Julia off by default.
		UT_Vector2T<float> joffset;

	public:
		Mandelbrot();
		Mandelbrot(int iter, float pow, float bailout, int jdepth, UT_Vector2T<float>joffset);

		virtual ~Mandelbrot();
		virtual int calculate(FCOORDS coords) override;
	};

}
