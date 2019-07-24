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
	// TODO: Push these to UI. This may end up not working well
	double start = 0.5;
	int max_value = 8192;

	int N = data.iters;
	double MIN = -1, MAX = 2;

	// Initialize N. TODO: Rename this horrible var
	double* X = new double[N + 1];
	X[0] = start;

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
		lmb = (int)(lmb / MIN * max_value);
	else
		lmb = (int)(lmb / MAX * max_value);

	return FractalCoordsInfo(lmb, lmb);
}

Lyapunov::~Lyapunov()
{
}
