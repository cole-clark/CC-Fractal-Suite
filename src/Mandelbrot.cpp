/*
	Cole Clark's Fractal Suite

	Mandelbrot.cpp
	Code for a Mandelbrot Fractal.
 */

#include <complex>

#include "Mandelbrot.h"



CC::Mandelbrot::Mandelbrot()
{
	joffset = 0;
}

CC::Mandelbrot::Mandelbrot(
	int maxiter, double fpow, double bailout, int jdepth, double joffset_x, double joffset_y) :
	maxiter(maxiter), fpow(fpow), bailout(bailout), jdepth(jdepth)
{
	// Create joffset as a complex number.
	joffset = COMPLEX(joffset_x, joffset_y);
}

CC::Mandelbrot::~Mandelbrot() {}

int CC::Mandelbrot::calculate(FCOORDS coords)
{
	COMPLEX z{ 0 };
	COMPLEX c{ coords.first, coords.second };

	int iterations{ 0 };

	// Forward declare
	int julia;

	// Iter here means Max Iterations.
	while (iterations < maxiter)
	{
		// Calculate Mandelbrot
		z = pow(z, fpow) + c;

		// Calculate Julias, if present. A jdepth of 1 is the canonical Julia Set.
		for (julia = 0; julia < jdepth; julia++)
			z = pow(z, fpow) + joffset;

		if (abs(z) > bailout)
			break;

		iterations++;
	}

	return iterations;
}
