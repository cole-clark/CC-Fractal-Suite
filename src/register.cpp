/*
	Cole Clark's Fractal Suite

	register.cpp
	Code that adds nodes from the dso to Houdini.
 */

#include <UT/UT_String.h>
 // Houdini never finds this dso without this included.
#include <UT/UT_DSOVersion.h>

#include "COP2_Buddhabrot.h"
#include "COP2_FractalMatte.h"
#include "COP2_Mandelbrot.h"
#include "COP2_Lyapunov.h"
#include "COP2_Pickover.h"

#include "FractalNode.h"

#include "register.h"

using namespace CC;

/// Installs the Fractal Generator Cop.
/// Houdini enforces this name for adding Cop2 Nodes.
void newCop2Operator(OP_OperatorTable* table)
{
	// TODO: Update other nodes to call 'CC::get_new_op<Node>'

	// Creates the Buddhabrot Definition
	OP_Operator* buddhabrot = new OP_Operator(
		"cc::fractal_buddhabrot",
		"CC Fractal Buddhabrot",
		COP2_Buddhabrot::myConstructor,
		&COP2_Buddhabrot::myTemplatePair,
		0,
		2, // optional mask input.
		&COP2_Buddhabrot::myVariablePair,
		0, // not generator
		COP2_Buddhabrot::myInputLabels);

	// Creates the Fractal Matte Definition
	OP_Operator* fractalMatte = new OP_Operator(
		"cc::fractal_matte",
		"CC Fractal Matte",
		COP2_FractalMatte::myConstructor,
		&COP2_FractalMatte::myTemplatePair,
		1,
		2, // optional mask input.
		&COP2_FractalMatte::myVariablePair,
		0, // not generator
		COP2_FractalMatte::myInputLabels);

	// Creates the Lyapunov Definition
	OP_Operator* lyapunov = new OP_Operator(
		"cc::fractal_lyapunov", // Node Name
		"CC Fractal Lyapunov", // Pretty Name
		COP2_Lyapunov::myConstructor,
		&COP2_Lyapunov::myTemplatePair,
		0,  // min inputs
		0,  // max inputs
		&COP2_Lyapunov::myVariablePair,
		OP_FLAG_GENERATOR);

	// Creates the Mandelbrot Definition
	OP_Operator* mandelbrot = new OP_Operator(
		"cc::fractal_mandelbrot", // Node Name
		"CC Fractal Mandelbrot", // Pretty Name
		COP2_Mandelbrot::myConstructor,
		&COP2_Mandelbrot::myTemplatePair,
		0,  // min inputs
		0,  // max inputs
		&COP2_Mandelbrot::myVariablePair,
		OP_FLAG_GENERATOR);

	// Creates the Pickover Definition
	OP_Operator* pickover = new OP_Operator(
		"cc::fractal_pickover", // Node Name
		"CC Fractal Pickover", // Pretty Name
		&OP::get_new_op<COP2_Pickover>,
		&COP2_Pickover::myTemplatePair,
		0,  // min inputs
		0,  // max inputs
		&COP2_Pickover::myVariablePair,
		OP_FLAG_GENERATOR);

	std::vector<OP_Operator*> nodes{
		buddhabrot, fractalMatte, lyapunov, mandelbrot, pickover };

	UT_String menuPath{ "Fractal" };
	for (auto node : nodes)
	{
		node->setOpTabSubMenuPath(menuPath);
		table->addOperator(node);
	}
}