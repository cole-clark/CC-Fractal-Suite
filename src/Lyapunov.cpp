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

	// TODO: Find a way to push this to the UI
	double seq[] = { a, b, b, a, b, a, a, b, b, a, b, a };
	int seq_size = 12;
	double start = 0.5;
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
		lmb = (int)(lmb / MIN * 10240);
	else
		lmb = (int)(lmb / MAX * 10240);

	return FractalCoordsInfo(lmb, lmb);
}

Lyapunov::~Lyapunov()
{
}
