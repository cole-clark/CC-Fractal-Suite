#pragma once

#include <complex>

#include <UT/UT_Vector2.h>

#include "Fractal.h"
#include "FractalSpace.h"

typedef std::complex<double> COMPLEX;

namespace CC
{
	/// Declare the mandelbrot to be used
	class Mandelbrot : public Fractal
	{
		int maxiter{ 0 };
		float fpow{ 2 };  // Fractal Pow
		float bailout{ 2 };
		int jdepth{ 0 };  // Julia off by default.
		COMPLEX joffset;

	public:
		Mandelbrot();
		Mandelbrot(int maxiter, float pow, float bailout, int jdepth, float joffset_x, float joffset_y);

		virtual ~Mandelbrot();
		virtual int calculate(FCOORDS coords) override;
	};

}
