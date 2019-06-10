#include "Fractal.h"

CC::Fractal::Fractal()
{
}

CC::Fractal::~Fractal()
{
}

CC::FractalCoordsInfo::FractalCoordsInfo(
	int num_iter,
	COMPLEX z) : num_iter(num_iter), z(z)
{
}
