/*
	Cole Clark's Fractal Suite

	FractalNode.cpp
	Code defining the FractalNode helper class.
 */

#include "FractalNode.h"
#include "FractalSpace.h"

using namespace CC;

void XformStashData::evalArgs(const OP_Node* node, fpreal t)
{
	scale = node->evalFloat(nameScale.getToken(), 0, t);
	offset_x = node->evalFloat(nameOffset.getToken(), 0, t);
	offset_y = node->evalFloat(nameOffset.getToken(), 1, t);
	rotate = node->evalFloat(nameRotate.getToken(), 0, t);
	xord = get_rst_order(node->evalInt(nameXOrd.getToken(), 0, t));
}
