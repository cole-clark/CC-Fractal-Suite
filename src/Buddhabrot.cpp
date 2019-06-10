/*
	Cole Clark's Fractal Suite

	Buddhabrot.cpp
	Code for a Buddhabrot Fractal.
 */

#include <complex>
#include <cmath>

#include "Buddhabrot.h"


CC::Buddhabrot::Buddhabrot()
{
	joffset = 0;
}

CC::Buddhabrot::Buddhabrot(
	int maxiter, double fpow, double bailout, int jdepth,
	double joffset_x, double joffset_y, int blackhole) :
	maxiter(maxiter), fpow(fpow), bailout(bailout), jdepth(jdepth), blackhole(blackhole)
{
	// Create joffset as a complex number.
	joffset = COMPLEX(joffset_x, joffset_y);
}

CC::Buddhabrot::~Buddhabrot() {}

int CC::Buddhabrot::calculate(FCOORDS coords)
{

	// Declares z and c where:Calculates the basic mandelbrot formula
	// z = z^pow + c;
	COMPLEX z{ 0 };
	COMPLEX c{ coords.first, coords.second };

	int iterations{ 0 };

	// Forward declare
	int julia;

	// Iter here means Max Iterations.
	while (iterations < maxiter)
	{
		// Calculate Buddhabrot
		z = pow(z, fpow) + c;

		// Calculate Julias, if present. A jdepth of 1 is the canonical Julia Set.
		for (julia = 0; julia < jdepth; julia++)
			z = pow(z, fpow) + joffset;

		if (abs(z) > bailout)
			break;

		iterations++;
	}

	// Blackhole if maximum iterations reached
	// Returns -1 for bailed out values, making it a unique value for mattes.
	if (blackhole && iterations == maxiter)
		iterations = -1;

	return iterations;
}