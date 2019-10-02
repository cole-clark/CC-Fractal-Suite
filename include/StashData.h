/** \file StashData.h
	Header that defines objects that stash cooked parameter data.

 * Cop2 Nodes rely on highly-threaded calls to calculate the image pixels.
 * As a result, Cops has a builting concept of a ContextData object that
 * is meant to hold onto the parameters, and is copied to each thread so that
 * parameters aren't cooked dozens of times. CCFS takes this one step further,
 * by using these StashData objects as though they were threadsafe 'parameter'
 * getters. This was necessary, because of how many different parms are shared
 * by nodes in the CCFS.
 */

#pragma once

 // Local
#include "typedefs.h"

// STL
#include <vector>
#include <initializer_list>

// HDK
#include <OP/OP_Node.h>

namespace CC
{
/** Base class for stash data defining pure virtual methods. */
class StashData
{
public:
	/** Method that sets parm values from node, based on time. */
	virtual void evalArgs(const OP_Node* node, fpreal t) = 0;
};

/** Struct that stashes of transformation parameter data for FractalSpace.
 * This can be natively used with the TEMPLATES_XFORM macro from FractalNode.h.
 */
struct XformStashData : public StashData
{
	/**< Translates*/
	fpreal offset_x{ 0 }, offset_y{ 0 };

	/**< Rotation value*/
	fpreal rotate{ 0 };

	/**< Uniform Scale value */
	fpreal scale{ 1 };

	/**< Transformation Order */
	RSTORDER xord{ RSTORDER::RST };

	XformStashData(
		fpreal offset_x = 0,
		fpreal offset_y = 0,
		fpreal rotate = 0,
		fpreal scale = 0,
		RSTORDER xord = RSTORDER::RST);

	void evalArgs(const OP_Node* node, fpreal t);
};

/** Struct that stashes a vector of XformStashDatas for FractalSpace.
 * This can be natively used with the TEMPLATES_XFORM_MULTI macro from
 * FractalNode.h.
*/
struct MultiXformStashData : StashData
{
	/** Vector of Xform populated by evalArgs.*/
	std::vector<XformStashData> xforms;

	void evalArgs(const OP_Node* node, fpreal t) override;
};

/** Struct that stashes the data required to construct a Mandelbrot Fractal. */
struct MandelbrotStashData : public StashData
{
	/**< Number of iterations. */
	int iters{ 50 };

	/**< Exponent calculated by z. */
	fpreal power{ 0 };

	/**< Value that 'escapes' the fractal set. */
	fpreal bailout{ 2 };

	/**< Number of times Julia set coorinates are iterated. */
	int jdepth{ 0 };

	/**< Offset Coordinates used by Julia set.*/
	COMPLEX joffset{ 0.0f, 0.0f };

	/**< Whether values that escape the set are colored black or not*/
	bool blackhole{ false };

	MandelbrotStashData(
		int iters = 50,
		fpreal power = 2,
		fpreal bailout = 2,
		int jdepth = 0,
		COMPLEX joffset = (0.0f, 0.0f),
		bool blackhole = false);

	void evalArgs(const OP_Node* node, fpreal t);
};

/** Struct that stashes the data required to create a Pickover Fractal.
 * This can be natively used by the TEMPLATES_PICKOVER macro in
 * FractalNode.h.
*/
struct PickoverStashData : public MandelbrotStashData
{
	/** Point position for the pickover's 'center' distance measurment point.*/
	COMPLEX popoint{ 0, 0 };

	/** Rotation of the pickover's line. */
	fpreal porotate{ 0 };

	/** Mode deciding whether the fractal measures against a point or line. */
	bool pomode{ false };

	/** Toggle for whether the reference point or line are displayed. */
	bool poref{ true };

	/** Size of the reference image point or line. */
	fpreal porefsize{ 10.0 };

	PickoverStashData(
		int iters = 50,
		fpreal power = 2.0,
		fpreal bailout = 2.0,
		int jdepth = 0,
		COMPLEX joffset = (0.0, 0.0),
		bool blackhole = false,
		COMPLEX popoint = (0.0, 0.0),
		fpreal porotate = 0.0,
		bool pomode = false,
		bool poref = true,
		fpreal porefsize = 10.0);

	void evalArgs(const OP_Node* node, fpreal t);
};

/** Struct that stashes the data required to create a Lyapunov Fractal.
 * This can be natively used by the TEMPLATES_Lyapunov macro in
 * FractalNode.h.
*/
struct LyapunovStashData : public StashData
{
	/** Number of iterations in Lyapunov fractal. */
	int iters{ 25 };

	/** The value of the first item in fractal. */
	fpreal start{ 0.5 };

	/** The highest value returned by the fractal. */
	fpreal maxval{ 5.0f };

	/** Toggles whether negative values return their absolute value. */
	bool invertnegative{ true };

	/** Sequence of 0-1 values, where 0 represents an X axis, 1 represents a
	 * Y axis, and intermediary values are in between. */
	std::vector<fpreal> seq;

	void evalArgs(const OP_Node* node, fpreal t);

	virtual ~LyapunovStashData();
};
}  // End of CC Namespace
