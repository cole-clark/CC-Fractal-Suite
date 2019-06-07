/*
	Cole Clark's Fractal Suite

	Mandelbrot.cpp
	Code for a Mandelbrot Fractal.
 */

#include <complex>
#include <cmath>

#include "Mandelbrot.h"


CC::Mandelbrot::Mandelbrot()
{
	joffset = 0;
}

CC::Mandelbrot::Mandelbrot(
	int maxiter, double fpow, double bailout, int jdepth,
	double joffset_x, double joffset_y, int blackhole) :
	maxiter(maxiter), fpow(fpow), bailout(bailout), jdepth(jdepth), blackhole(blackhole)
{
	// Create joffset as a complex number.
	joffset = COMPLEX(joffset_x, joffset_y);
}

CC::Mandelbrot::~Mandelbrot() {}

int CC::Mandelbrot::calculate(FCOORDS coords)
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
		// Calculate Mandelbrot
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

double CC::Mandelbrot::calculate_orbit_trap(FCOORDS coords)
{
	COMPLEX z{ 0 };
	COMPLEX c{ coords.first, coords.second };

	double distance{ 1e10 };

	for (int i = 0; i < maxiter; i++)
	{
		// Calculate Mandelbrot
		z = pow(z, fpow) + c;

		// Calculate Julias, if present. A jdepth of 1 is the canonical Julia Set.
		for (int julia = 0; julia < jdepth; julia++)
			z = pow(z, fpow) + joffset;


		COMPLEX zMinusPoint{ 0 };
		zMinusPoint -= z;

		double zMinusPointModulus = std::sqrt(
			std::pow(z.imag() - z.real(), 2) + 
			std::pow(zMinusPoint.imag() - zMinusPoint.real(), 2));

		if (zMinusPointModulus < distance)
			distance = zMinusPointModulus;
	}

	return distance;
}