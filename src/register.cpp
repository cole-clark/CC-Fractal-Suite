/*
	Cole Clark's Fractal Suite

	register.cpp
	Code that adds nodes from the dso to Houdini.
 */

#include "COP2_Mandelbrot.h"

#include "register.h"


/// Installs the Fractal Generator Cop.
/// Houdini enforces this name for adding Cop2 Nodes.
void newCop2Operator(OP_OperatorTable* table)
{
	// Creates the mandelbrot Definition
	table->addOperator( new OP_Operator(
		"cc::fractal_mandelbrot", // Node Name
		"CC Fractal Mandelbrot", // Pretty Name
		CC::COP2_Mandelbrot::myConstructor,
		&CC::COP2_Mandelbrot::myTemplatePair,
		0,  // min inputs
		2,  // max inputs. Second is for mask.
		&CC::COP2_Mandelbrot::myVariablePair,
		OP_FLAG_GENERATOR
		));
}
