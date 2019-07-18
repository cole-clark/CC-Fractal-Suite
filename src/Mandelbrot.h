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
		MandelbrotStashData mdata;

		Mandelbrot(
			int iters = 50,
			double power = 2,
			double bailout = 4,
			int jdepth = 0,
			COMPLEX joffset = (0, 0),
			bool blackhole = false);

		Mandelbrot(MandelbrotStashData& mandelData);

		virtual ~Mandelbrot();
		virtual FractalCoordsInfo calculate(COMPLEX coords) override;
		COMPLEX calculate_z(COMPLEX z, COMPLEX c);

		// Temp Placement
		double calculate_orbit_trap(COMPLEX coords);
		FractalCoordsInfo calculate_lyapunov(COMPLEX coords);
	};
}
