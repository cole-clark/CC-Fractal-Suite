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
	std::vector<double> seq = generate_sequence(
		data.seq, coords.real(), coords.imag());

	int N = data.iters;

	double* X = new double[N + 1];
	X[0] = data.start;

	for (int n = 1; n <= N; n++)
	{
		X[n] = seq[n - 1 % data.seq.size()] * X[n - 1] * (1.0 - X[n - 1]);
	}

	// Calculate Lyapunov
	double lmb{ 0 };

	for (int n = 1; n <= N; n++)
	{
		lmb += log(abs(seq[n % data.seq.size()] * (1.0 - 2.0 * X[n]))) / log(2);
	}
	lmb /= N;

	if (isinf(lmb))
		lmb = 0.0f;

	delete X;

	if (lmb < 0)
		lmb = (int)(lmb / data.minmax[0] * data.maxval);
	else
		lmb = (int)(lmb / data.minmax[1] * data.maxval);

	return FractalCoordsInfo(lmb, lmb);
}

std::vector<double>
Lyapunov::generate_sequence(std::vector<double> sequence, double x, double y)
{
	std::vector<double> vals;

	for (int i = 0; i < sequence.size(); ++i)
	{
		vals.emplace_back(SYSlerp(x, y, sequence[i]));
	}

	return vals;
}

Lyapunov::~Lyapunov()
{
}
