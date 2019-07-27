/*
	Cole Clark's Fractal Suite

	Lyapunov.cpp
	Code for a Lyapunov Fractal.
 */

#include "Lyapunov.h"

using namespace CC;

Lyapunov::Lyapunov(LyapunovStashData & lyaData)
{
	data = lyaData;
}

FractalCoordsInfo Lyapunov::calculate(COMPLEX coords)
{
	auto preseq = generate_sequence(coords.real(), coords.imag());

	std::vector<double> seq;
	seq.reserve(data.iters + 1);
	seq[0] = data.start;

	for (int i = 1; i <= data.iters; i++)
	{
		seq[i] = 
			preseq[i - 1 % data.seq.size()] *
			seq[i - 1] *
			(1.0 - seq[i - 1]);
	}

	// Calculate Lyapunov
	double value{ 0 };

	for (int i = 1; i <= data.iters; i++)
	{
		value += log(abs(
			preseq[i % data.seq.size()]
			* (1.0 - 2.0 * seq[i]))) / log(2);
	}
	value /= data.iters;

	if (isinf(abs(value)))
		value = 0.0f;

	if (value < 0)
		value = (int)(value / data.minmax[0] * data.maxval);
	else
		value = (int)(value / data.minmax[1] * data.maxval);

	return FractalCoordsInfo(0, 0, value);
}

/// Returns a sequence of values lerped between the x and y coordinates.
std::vector<double>
Lyapunov::generate_sequence(double x, double y)
{
	std::vector<double> vals;

	for (int i = 0; i < data.seq.size(); ++i)
	{
		vals.emplace_back(SYSlerp(x, y, data.seq[i]));
	}

	return vals;
}

Lyapunov::~Lyapunov()
{
}
