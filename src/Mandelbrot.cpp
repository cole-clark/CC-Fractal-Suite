/*
	Cole Clark's Fractal Suite

	Mandelbrot.cpp
	Code for a Mandelbrot Fractal.
 */

#include <complex>
#include <vector>

#include "Mandelbrot.h"

using namespace CC;

Mandelbrot::Mandelbrot(
	int iters, double power, double bailout,
	int jdepth, COMPLEX joffset,
	bool blackhole)
{
	mdata = MandelbrotStashData(
		iters, power, bailout,
		jdepth, joffset,
		blackhole);
}

Mandelbrot::Mandelbrot(MandelbrotStashData & mandelData)
{
	this->mdata = mandelData;
}

Mandelbrot::~Mandelbrot() {}

FractalCoordsInfo CC::Mandelbrot::calculate(COMPLEX coords)
{
	// Declares z and c where:Calculates the basic mandelbrot formula
	// z = z^pow + c;
	COMPLEX z{ 0 };
	COMPLEX c{ coords.real(), coords.imag()};

	int iterations{ 0 };
	double smoothcolor = exp(-abs(-z));

	while (iterations < mdata.iters)
	{
		z = calculate_z(z, c);
		smoothcolor += exp(-abs(-z));

		if (abs(z) > mdata.bailout)
			break;

		++iterations;
	}

	// Blackhole if maximum iterations reached
	// Itersations set to -1 for bailed out values, making it a unique value for mattes.
	if (mdata.blackhole && iterations == mdata.iters)
	{
		iterations = -1;
		smoothcolor = -1;
	}
	return FractalCoordsInfo(iterations, z, smoothcolor);
}

COMPLEX Mandelbrot::calculate_z(COMPLEX z, COMPLEX c)
{
	// Calculate Mandelbrot
	z = pow(z, mdata.power) + c;

	// Calculate Julias, if present. A jdepth of 1 is the canonical Julia Set.
	for (int julia = 0; julia < mdata.jdepth; julia++)
		z = pow(z, mdata.power) + mdata.joffset;

	return z;
}

FractalCoordsInfo Mandelbrot::calculate_pickover(COMPLEX coords)
{
	COMPLEX z{ 0 };
	COMPLEX c{ coords.real(), coords.imag() };

	double distance{ 1e10 };

	for (int i = 0; i < mdata.iters; i++)
	{
		// Calculate Mandelbrot
		z = pow(z, mdata.power) + c;

		// Calculate Julias, if present. A jdepth of 1 is the canonical Julia Set.
		for (int julia = 0; julia < mdata.jdepth; julia++)
			z = pow(z, mdata.power) + mdata.joffset;


		COMPLEX zMinusPoint{ 0 }; // TODO: Move to interface
		zMinusPoint -= z;

		double zMinusPointModulus = std::sqrt(
			std::pow(z.imag() - z.real(), 2) + 
			std::pow(zMinusPoint.imag() - zMinusPoint.real(), 2));

		if (zMinusPointModulus < distance)
			distance = zMinusPointModulus;
	}

	return FractalCoordsInfo(0, 0, distance);
}

FractalCoordsInfo
Mandelbrot::calculate_lyapunov(COMPLEX coords)
{
	double a = coords.real();
	double b = coords.imag();

	// TODO: Find a way to push this to the UI
	double seq[] = { a, b, b, a, b, a, a, b, b, a, b, a };
	int seq_size = 12;
	double start = 0.5;
	int N = mdata.iters;
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
