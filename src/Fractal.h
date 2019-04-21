/*
	Cole Clark's Fractal Suite

	Fractal.h
	Header defining CC fractal base class.
 */

#pragma once

#include "FractalSpace.h"

namespace CC
{
	class Fractal
	{
	public:
		Fractal();
		virtual ~Fractal();

		virtual int calculate(FCOORDS coords) = 0;
	};
}