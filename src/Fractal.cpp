#include "Fractal.h"

CC::Fractal::Fractal()
{
}

CC::Fractal::~Fractal()
{
}

CC::FractalCoordsInfo::FractalCoordsInfo(int num_iter, COMPLEX z, double smooth) :
	num_iter(num_iter), z(z), smooth(smooth) {}

