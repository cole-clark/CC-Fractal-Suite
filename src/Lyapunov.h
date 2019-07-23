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

		FractalCoordsInfo calculate(COMPLEX coords);
		virtual ~Lyapunov();

	};
}