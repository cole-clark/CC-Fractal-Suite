/*
	Cole Clark's Fractal Suite

	Lyapunov.cpp
	Header for a Lyapunov Fractal.
 */

#pragma once

#include "Fractal.h"
#include "FractalSpace.h"

namespace CC
{
	class Lyapunov : public Fractal
	{
	public:
		LyapunovStashData data;

		Lyapunov() = default;
		Lyapunov(LyapunovStashData& lyaData);
		virtual ~Lyapunov();

		FractalCoordsInfo calculate(COMPLEX coords);
		std::vector<double> generate_sequence(double x, double y);
	};
}