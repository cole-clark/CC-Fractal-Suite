/*
	Cole Clark's Fractal Suite

	Mandelbrot.h
	Header for Mandelbrot fractal class.
 */

#pragma once

#include <complex>

#include <UT/UT_Vector2.h>

#include "Fractal.h"
#include "FractalSpace.h"

namespace CC
{
	/// Declare the mandelbrot to be used
	class Mandelbrot : public Fractal
	{
	public:
		int maxiter{ 0 };
		double fpow{ 2 };  // Fractal Pow
		double bailout{ 2 };
		int jdepth{ 0 };  // Julia off by default.
		COMPLEX joffset;
		bool blackhole{ false };

		Mandelbrot();
		Mandelbrot(
			int maxiter,
			double pow,
			double bailout,
			int jdepth,
			double joffset_x,
			double joffset_y,
			int blackhole);

		virtual ~Mandelbrot();
		virtual FractalCoordsInfo calculate(COMPLEX coords) override;
		COMPLEX calculate_z(COMPLEX z, COMPLEX c);

		// Temp Placement
		double calculate_orbit_trap(COMPLEX coords);
		double calculate_smooth(COMPLEX coords);
	};
}
