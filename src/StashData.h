/*
	Cole Clark's Fractal Suite

	StashData.h
	Header defining stashed data.
 */

#pragma once

#include "typedefs.h"

#include <OP/OP_Node.h>

 /// Struct meant to simplify the stashing of xform parm data.
struct XformStashData
{
	double offset_x, offset_y{ 0 };
	double rotate{ 0 };
	double scale{ 1 };

	RSTORDER xord{ RSTORDER::TRS };

	void evalArgs(const OP_Node* node, fpreal t);
};