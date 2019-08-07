/*
	Cole Clark's Fractal Suite

	Mandelbrot.cpp
	Code for a Mandelbrot Fractal.
 */

#include <complex>
#include <vector>

#include "Mandelbrot.h"

using namespace CC;

Mandelbrot::Mandelbrot(MandelbrotStashData& mandelData)
{
	data = mandelData;
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

	while (iterations < data.iters)
	{
		z = calculate_z(z, c);
		smoothcolor += exp(-abs(-z));

		if (abs(z) > data.bailout)
			break;

		++iterations;
	}

	// Blackhole if maximum iterations reached
	// Itersations set to -1 for bailed out values, making it a unique value for mattes.
	if (data.blackhole && iterations == data.iters)
	{
		iterations = -1;
		smoothcolor = -1;
	}
	return FractalCoordsInfo(iterations, z, smoothcolor);
}

COMPLEX Mandelbrot::calculate_z(COMPLEX z, COMPLEX c)
{
	// Calculate Mandelbrot
	z = pow(z, data.power) + c;

	// Calculate Julias, if present. A jdepth of 1 is the canonical Julia Set.
	for (int julia = 0; julia < data.jdepth; julia++)
		z = pow(z, data.power) + data.joffset;

	return z;
}

Pickover::Pickover(PickoverStashData & pickoverData)
{
	data = pickoverData;
}

FractalCoordsInfo Pickover::calculate(COMPLEX coords)
{
	COMPLEX z{ 0 };
	COMPLEX c{ coords.real(), coords.imag() };

	double distance{ 1e10 };

	for (int i = 0; i < data.iters; i++)
	{
		// Calculate Mandelbrot
		z = pow(z, data.power) + c;

		// Calculate Julias, if present. A jdepth of 1 is the canonical Julia Set.
		for (int julia = 0; julia < data.jdepth; julia++)
			z = pow(z, data.power) + data.joffset;

		// Calculate the distance
		double zLength{ 0 };
		if (data.pomode == 0) // Calculate Point
			zLength = distance_to_point(z, data.popoint);
		else  // Calculate Line
			zLength = distance_to_line(z, data.popoint, data.porotate);

		// Assign distance if a smaller length than current distance.
		if (zLength < distance)
			distance = zLength;
	}

	return FractalCoordsInfo(0, 0, distance);
}

double Pickover::distance_to_point(COMPLEX z, COMPLEX point)
{
	COMPLEX zDist{ 0 };
	zDist = point - z;

	// Calculates the magnitude of the complex number
	// sqrt(a^2 + b^2)
	double distance = std::sqrt(
		std::pow(z.imag() - z.real(), 2) +
		std::pow(zDist.imag() - zDist.real(), 2));

	return distance;
}

double Pickover::distance_to_line(COMPLEX z, COMPLEX offset, fpreal theta)
{
	// Create 2D point Transformation Matrices that will be our line
	UT_Matrix3T<fpreal> mA, mB;
	mA.identity();
	// First translate line 'A'
	mA.xform(RSTORDER::TRS, offset.real(), offset.imag());
	// Assign 'B' to 'A', and offset by one in the x axis
	mB = mA;
	mB.translate({ 1, 0 });
	// Rotate 'B' around A
	mB.xform(RSTORDER::TRS, 0.0, 0.0, theta, 1.0, 1.0, offset.real(), offset.imag());

	// Extract point positions from which we will draw our line.
	COMPLEX pntA{ mA(2, 0), mA(2, 1) };
	COMPLEX pntB{ mB(2, 0), mB(2, 1) };

	// Calculate distance from point to line.
	// Math taken from line defined by equation at:
	// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line

	double numerator = abs(
		(pntB.imag() - pntA.imag()) * z.real() -
		(pntB.real() - pntA.real()) * z.imag() +
		pntB.real() * pntA.imag() - pntB.imag() * pntA.real()
	);

	double denominator = sqrt(
		pow(pntB.imag() - pntA.imag(), 2) +
		pow(pntB.real() - pntA.real(), 2)
	);

	if (denominator == 0.0)
		denominator = 1.0f;

	return numerator / denominator;
}
