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
		MandelbrotStashData data;

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
	};

	class Pickover : public Mandelbrot
	{
	public:
		PickoverStashData data;

		Pickover(
			int iters = 50,
			double power = 2,
			double bailout = 4,
			int jdepth = 0,
			COMPLEX joffset = (0, 0),
			bool blackhole = false,
			COMPLEX popoint = (0.0f, 0.0f),
			double porotate = 0.0f,
			int pomode = false);

		Pickover(PickoverStashData& pickoverData);
	
		virtual FractalCoordsInfo calculate(COMPLEX coords);

		/// Where z is the current iteration, and point is a reference point.
		double distance_to_point(COMPLEX z, COMPLEX point);
		double distance_to_line(COMPLEX z, COMPLEX offset, fpreal theta);
	};
}
