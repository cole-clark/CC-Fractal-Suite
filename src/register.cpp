/*
	Cole Clark's Fractal Suite

	register.cpp
	Code that adds nodes from the dso to Houdini.
 */

 // Houdini never finds this dso without this included.
#include <UT/UT_DSOVersion.h>

#include "COP2_Mandelbrot.h"
#include "COP2_FractalMatte.h"

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

	table->addOperator(new OP_Operator(
		"cc::fractal_matte",
		"CC Fractal Matte",
		CC::COP2_FractalMatte::myConstructor,
		&CC::COP2_FractalMatte::myTemplatePair,
		1,
		2, // optional mask input.
		&CC::COP2_FractalMatte::myVariablePair,
		0, // not generator
		CC::COP2_FractalMatte::myInputLabels));
}