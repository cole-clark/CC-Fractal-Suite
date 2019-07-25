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
	double a = coords.real();
	double b = coords.imag();
	double c = SYSlerp(a, b, 0.5f);
	// TODO: Find a way to push this to the UI
	double seq[] = { a, b, c, a, c, a, a, b, c, a, b, c };
	int seq_size = 12;

	int N = data.iters;

	// Initialize N. TODO: Rename this horrible var
	double* X = new double[N + 1];
	X[0] = data.start;

	for (int n = 1; n <= N; n++)
	{
		X[n] = seq[n - 1 % seq_size] * X[n - 1] * (1.0 - X[n - 1]);
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
		lmb = (int)(lmb / data.minmax[0] * data.maxval);
	else
		lmb = (int)(lmb / data.minmax[1] * data.maxval);

	return FractalCoordsInfo(lmb, lmb);
}

Lyapunov::~Lyapunov()
{
}
