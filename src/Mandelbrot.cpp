/*
	Cole Clark's Fractal Suite

	Mandelbrot.cpp
	Code for a Mandelbrot Fractal.
 */

#include "Mandelbrot.h"

CC::Mandelbrot::Mandelbrot()
{
	joffset = 0;
}

CC::Mandelbrot::Mandelbrot(
	int iter, float pow, float bailout, int jdepth, UT_Vector2T<float> joffset) :
	iter(iter), pow(pow), bailout(bailout), jdepth(jdepth), joffset(joffset) { }

CC::Mandelbrot::~Mandelbrot()
{
}

// TODO : Implement
int CC::Mandelbrot::calculate(FCOORDS coords)
{
	return 0;
}
