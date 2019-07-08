/*
	Cole Clark's Fractal Suite

	Mandelbrot.cpp
	Code for a Mandelbrot Fractal.
 */

#include <complex>
#include <vector>

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

CC::FractalCoordsInfo CC::Mandelbrot::calculate(COMPLEX coords)
{
	// Declares z and c where:Calculates the basic mandelbrot formula
	// z = z^pow + c;
	COMPLEX z{ 0 };
	COMPLEX c{ coords.real(), coords.imag()};

	int iterations{ 0 };
	double smoothcolor = exp(-abs(-z));

	// Iter here means Max Iterations.
	while (iterations < maxiter)
	{
		z = calculate_z(z, c);
		smoothcolor += exp(-abs(-z));

		if (abs(z) > bailout)
			break;

		iterations++;
	}

	// Blackhole if maximum iterations reached
	// Itersations set to -1 for bailed out values, making it a unique value for mattes.
	if (blackhole && iterations == maxiter)
	{
		iterations = -1;
		smoothcolor = -1;
	}
	return FractalCoordsInfo(iterations, z, smoothcolor);
}

COMPLEX CC::Mandelbrot::calculate_z(COMPLEX z, COMPLEX c)
{
	// Calculate Mandelbrot
	z = pow(z, fpow) + c;

	// Calculate Julias, if present. A jdepth of 1 is the canonical Julia Set.
	for (int julia = 0; julia < jdepth; julia++)
		z = pow(z, fpow) + joffset;

	return z;
}

double CC::Mandelbrot::calculate_orbit_trap(COMPLEX coords)
{
	COMPLEX z{ 0 };
	COMPLEX c{ coords.real(), coords.imag() };

	double distance{ 1e10 };

	for (int i = 0; i < maxiter; i++)
	{
		// Calculate Mandelbrot
		z = pow(z, fpow) + c;

		// Calculate Julias, if present. A jdepth of 1 is the canonical Julia Set.
		for (int julia = 0; julia < jdepth; julia++)
			z = pow(z, fpow) + joffset;


		COMPLEX zMinusPoint{ 0 }; // TODO: Move to interface
		zMinusPoint -= z;

		double zMinusPointModulus = std::sqrt(
			std::pow(z.imag() - z.real(), 2) + 
			std::pow(zMinusPoint.imag() - zMinusPoint.real(), 2));

		if (zMinusPointModulus < distance)
			distance = zMinusPointModulus;
	}

	return distance;
}

double CC::Mandelbrot::calculate_smooth(COMPLEX coords)
{
	// Declares z and c where:Calculates the basic mandelbrot formula
	// z = z^pow + c;
	COMPLEX z{ 0 };
	COMPLEX c{ coords.real(), coords.imag() };

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



	/*
  // Used to avoid floating point issues with points inside the set.
  if ( iteration < max_iteration ) {
	// sqrt of inner term removed using log simplification rules.
	log_zn = log( x*x + y*y ) / 2
	nu = log( log_zn / log(2) ) / log(2)
	// Rearranging the potential function.
	// Dividing log_zn by log(2) instead of log(N = 1<<8)
	// because we want the entire palette to range from the
	// center to radius 2, NOT our bailout radius.
	iteration = iteration + 1 - nu
  }
  color1 = palette[floor(iteration)]
  color2 = palette[floor(iteration) + 1]
  // iteration % 1 = fractional part of iteration.
  color = linear_interpolate(color1, color2, iteration % 1)
  plot(Px, Py, color)
	
	*/

	double color = (double)iterations;
	/*
	// Kind of Functional
	if (iterations < maxiter)
	{
		double log_zn = std::log(
			std::pow(z.real(), 2) +
			std::pow(z.imag(), 2)) / 2;
		//double nu = std::log(log_zn / std::log(2)) / std::log(2);
		double nu = std::log(log_zn / std::log(fpow)) / std::log(fpow);
		// Adding one here ensures blending into '0' bands of the fractal.
		color = iterations + 1 - nu;
	}
	*/
	if (iterations < maxiter)
	{
		double log_zn = std::log(z.real() + z.imag());
		color = log_zn;
	}

	return color;
}

CC::FractalCoordsInfo
CC::Mandelbrot::calculate_lyapunov(COMPLEX coords)
{
	double a = coords.real();
	double b = coords.imag();

	// TODO: Find a way to push this to the UI
	double seq[] = { a, b, b, a, b, a, a, b, b, a, b, a };
	int seq_size = 12;
	double start = 0.5;
	int N = maxiter;
	double MIN = -1, MAX = 2;

	// Initialize N. TODO: Rename this horrible var
	double* X = new double[N + 1];
	X[0] = start;

	for (int n = 1; n <= N; n++)
	{
		X[n] = seq[n-1 % seq_size] * X[n-1] * (1.0-X[n-1]);
	}

	// Calculate Lyapunov
	double lmb{ 0 };

	for (int n = 1; n <= N; n++)
	{
		lmb += log(abs(seq[n % seq_size] * (1.0 - 2.0 * X[n]))) / log(2);
	}
	lmb /= N;

	if (isinf(lmb))
		lmb = 0.0f;

	delete X;

	if (lmb < 0)
		lmb = (int)(lmb / MIN * 10240);
	else
		lmb = (int)(lmb / MAX * 10240);

	return FractalCoordsInfo(lmb, lmb);
}
