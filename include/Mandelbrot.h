/** \file Mandelbrot.h
	Header declaring the Mandelbrot and Pickover fractal classes.
 */

#pragma once

 // Local
#include "Fractal.h"
#include "FractalSpace.h"

// STL
#include <complex>

// HDK
#include <UT/UT_Vector2.h>


namespace CC
{
/**Class implementing the Mandelbrot fractal.
 * It is being treated as a 'principled mandelbrot', where as far as
 * is sensibles, the formula was opened up and parameterized so that it
 * can be animated, meaning that it is easily capable of creating fractals
 * that are not strictly fractals. Julia sets, multibrots, and the ability
 * to animate between them is expressed through this object.*/
class Mandelbrot : public Fractal
{
public:
	MandelbrotStashData data;

	Mandelbrot() = default;
	Mandelbrot(MandelbrotStashData& mandelData);

	virtual ~Mandelbrot();

	/**Calculates the Mandelbrot fractal.*/
	virtual FractalCoordsInfo calculate(COMPLEX coords) override;

	/**Calculate Z runs only the fractal math calculation. This is
	 * distinct from the calculate member in that calculate initializes
	 * some data, and sets the return type conditions. This has been
	 * separated so that this class can be subclassed, and still use
	 * Mandelbrot-like fractals without duplicating the fundamental math.*/
	COMPLEX calculate_z(COMPLEX z, COMPLEX c);
};

/**Class that implements the 'Pickover Stalk' fractal. In fractal terms, it
 * is calculated the same way as a Mandelbrot, but renders the fractal data
 * differently. While Mandelbrot returns a number of iterations, Pickover
 * measures the distance from a number in the Mandelbrot set to
 * user-specified 2-D geometry. The CCFS has implemented Point and line as
 * types to measure distance against.*/
class Pickover : public Mandelbrot
{
public:
	/**PickoverStashData, like Pickover, is subclassed from a Mandelbrot
	 * related object. */
	PickoverStashData data;

	Pickover() = default;
	Pickover(PickoverStashData& pickoverData);

	virtual FractalCoordsInfo calculate(COMPLEX coords);

	/** Z is the current iteration in a recursive fractal calculation,
	 * and point is a reference point. */
	fpreal distance_to_point(COMPLEX z, COMPLEX point);

	/** Z is the current iteration in a recursive fractal calculation,
	 * offset is a translation that will be applied to the line, and theta
	 * is a number of degrees to rotate the line.*/
	fpreal distance_to_line(COMPLEX z, COMPLEX offset, fpreal theta);
};
}
