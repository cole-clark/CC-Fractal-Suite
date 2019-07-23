/*
	Cole Clark's Fractal Suite

	StashData.cpp
	Code implementing stashed data.
 */

#include "StashData.h"
#include "typedefs.h"
#include "FractalSpace.h"

using namespace CC;

XformStashData::XformStashData(
	double offset_x, double offset_y,
	double rotate, double scale, RSTORDER xord) : 
		offset_x(offset_x), offset_y(offset_y),
		rotate(rotate), scale(scale), xord(xord) {}

void XformStashData::evalArgs(const OP_Node* node, fpreal t)
{
	scale = node->evalFloat(SCALE_NAME.first, 0, t);
	offset_x = node->evalFloat(TRANSLATE_NAME.first, 0, t);
	offset_y = node->evalFloat(TRANSLATE_NAME.first, 1, t);
	rotate = node->evalFloat(ROTATE_NAME.first, 0, t);
	xord = get_rst_order(node->evalInt(XORD_NAME.first, 0, t));
}

MandelbrotStashData::MandelbrotStashData(
	int iters, double power, double bailout,
	int jdepth, COMPLEX joffset,
	bool blackhole) :
		iters(iters), power(power), bailout(bailout),
		jdepth(jdepth),
		joffset(joffset),
		blackhole(blackhole)
{}

void MandelbrotStashData::evalArgs(const OP_Node * node, fpreal t)
{
	iters = node->evalInt(ITERS_NAME.first, 0, t);
	power = node->evalFloat(POWER_NAME.first, 0, t);
	bailout = node->evalFloat(BAILOUT_NAME.first, 0, t);
	jdepth = node->evalInt(JDEPTH_NAME.first, 0, t);
	double joffset_x = node->evalFloat(JOFFSET_NAME.first, 0, t);
	double joffset_y = node->evalFloat(JOFFSET_NAME.first, 1, t);
	joffset = COMPLEX(joffset_x, joffset_y);
	blackhole = node->evalInt(JOFFSET_NAME.first, 1, t) == 1;  // makes boolean
}

PickoverStashData::PickoverStashData(
	int iters, double power, double bailout,
	int jdepth, COMPLEX joffset, bool blackhole,
	COMPLEX popoint, double porotate, bool pomode) :
	MandelbrotStashData(iters, power, bailout, jdepth, joffset, blackhole),
	popoint(popoint), porotate(porotate), pomode(pomode)
{}

void PickoverStashData::evalArgs(const OP_Node * node, fpreal t)
{
	// Call the mandelbrot stash values first
	MandelbrotStashData::evalArgs(node, t);

	// Call pickover-specific methods.
	double popoint_x = node->evalFloat(POPOINT_NAME.first, 0, t);
	double popoint_y = node->evalFloat(POPOINT_NAME.first, 1, t);
	popoint = COMPLEX(popoint_x, popoint_y);
	porotate = node->evalFloat(POROTATE_NAME.first, 0, t);
	pomode = node->evalInt(POMODE_NAME.first, 0, t);
}

void LyapunovStashData::evalArgs(const OP_Node * node, fpreal t)
{

}
