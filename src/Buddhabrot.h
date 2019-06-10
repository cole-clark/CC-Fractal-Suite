/*
	Cole Clark's Fractal Suite

	Buddhabrot.h
	Header for Buddhabrot fractal class.
 */

#pragma once

#include <complex>

#include "Fractal.h"
#include "FractalSpace.h"

typedef std::complex<double> COMPLEX;

namespace CC
{
	/// Declare the mandelbrot to be used
	class Buddhabrot : public Fractal
	{
		// From Buddhabrot
		FractalSpace space;
		int samples{ 1 };

		// From Mandelbrot
		int maxiter{ 0 };
		double fpow{ 2 };  // Fractal Pow
		double bailout{ 2 };
		int jdepth{ 0 };  // Julia off by default.
		COMPLEX joffset;
		bool blackhole{ false };

	public:
		Buddhabrot();
		Buddhabrot(
			FractalSpace& space,
			int samples,
			int maxiter,
			double pow,
			double bailout,
			int jdepth,
			double joffset_x,
			double joffset_y,
			int blackhole);

		virtual ~Buddhabrot();
		virtual int calculate(FCOORDS coords) override;
	};
}
