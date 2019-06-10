/*
	Cole Clark's Fractal Suite

	Fractal.h
	Header defining CC fractal base class.
 */

#pragma once

#include "typedefs.h"
#include "FractalSpace.h"

namespace CC
{
	struct FractalCoordsInfo
	{
		int num_iter{ 0 };
		COMPLEX z;

		FractalCoordsInfo() = default;
		FractalCoordsInfo(int num_iter, COMPLEX z);
	};


	class Fractal
	{
	public:
		Fractal();
		virtual ~Fractal();

		/// Returns Fractal Info for a particular set of coords
		virtual FractalCoordsInfo calculate(COMPLEX coords) = 0;
	};
}