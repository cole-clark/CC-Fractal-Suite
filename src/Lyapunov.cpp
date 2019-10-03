/** \file Lyapunov.cpp
	Source declaring the Lyapunov fractal class.
 */

 // Local
#include "Lyapunov.h"

CC::Lyapunov::Lyapunov(LyapunovStashData & lyaData)
{
	data = lyaData;
}

CC::FractalCoordsInfo
CC::Lyapunov::calculate(COMPLEX coords)
{
	auto preseq = generate_sequence(coords.real(), coords.imag());

	std::vector<fpreal> seq;

	int niters = data.iters;
	if (niters > preseq.size())
		niters = preseq.size();

	seq.reserve(data.iters + 1);
	seq[0] = data.start;

	for (int i = 1; i <= niters; i++)
	{
		seq[i] =
			preseq[i - 1 % data.seq.size()] *
			seq[i - 1] *
			(1.0 - seq[i - 1]);
	}

	// Calculate Lyapunov
	fpreal value{ 0 };
	fpreal log2mult = 1.0 / SYSlog(2.0);
	for (int i = 1; i <= niters; i++)
	{
		value += SYSlog(SYSabs(
			preseq[i % data.seq.size()] *
			(1.0 - 2.0) * seq[i])) * log2mult;
	}
	value /= niters;

	if (value < 0)
		if (data.invertnegative)
			value *= -1;

	if (value > data.maxval)
		value = data.maxval;

	value /= data.maxval;

	return FractalCoordsInfo(0, 0, value);
}

std::vector<fpreal>
CC::Lyapunov::generate_sequence(fpreal x, fpreal y)
{
	std::vector<fpreal> vals;

	for (int i = 0; i < data.seq.size(); ++i)
	{
		vals.emplace_back(SYSlerp(x, y, data.seq[i]));
	}

	return vals;
}

CC::Lyapunov::~Lyapunov()
{
}
