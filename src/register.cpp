/** \file register.cpp
	Source that declares new(Context)Operator functions.

 * Houdini automatically calls these methods to add our Operators to the
 * operator table. Therefore, these are kept in the global namespace as the
 * usage of these functions do not belong to us.
 */

 // Local
#include "FractalNode.h"
#include "register.h"
#include "COP2_Buddhabrot.h"
#include "COP2_FractalMatte.h"
#include "COP2_Mandelbrot.h"
#include "COP2_Lyapunov.h"
#include "COP2_Pickover.h"

// HDK
/** Only one UT_DSOVersion include is allowed per-plugin. */
#include <UT/UT_DSOVersion.h>
#include <UT/UT_String.h>
#include <OP/OP_OperatorTable.h>

 /** Registers Cop2 nodes with Houdini. */
void newCop2Operator(OP_OperatorTable* table)
{
	/** Creates the Buddhabrot Definition */
	OP_Operator* buddhabrot = new OP_Operator(
		"cc::fractal_buddhabrot",
		"CC Fractal Buddhabrot",
		&CC::OP::get_new_op<CC::COP2_Buddhabrot>,
		&CC::COP2_Buddhabrot::myTemplatePair,
		0,
		2, // optional mask input.
		&CC::COP2_Buddhabrot::myVariablePair,
		0, // not generator
		CC::COP2_Buddhabrot::myInputLabels);

	/** Creates the Fractal Matte Definition */
	OP_Operator* fractalMatte = new OP_Operator(
		"cc::fractal_matte",
		"CC Fractal Matte",
		&CC::OP::get_new_op<CC::COP2_FractalMatte>,
		&CC::COP2_FractalMatte::myTemplatePair,
		1,
		2, // optional mask input.
		&CC::COP2_FractalMatte::myVariablePair,
		0, // not generator
		CC::COP2_FractalMatte::myInputLabels);

	/** Creates the Lyapunov Fractal Definition */
	OP_Operator* lyapunov = new OP_Operator(
		"cc::fractal_lyapunov", // Node Name
		"CC Fractal Lyapunov", // Pretty Name
		&CC::OP::get_new_op<CC::COP2_Lyapunov>,
		&CC::COP2_Lyapunov::myTemplatePair,
		0,  // min inputs
		0,  // max inputs
		&CC::COP2_Lyapunov::myVariablePair,
		OP_FLAG_GENERATOR);

	/** Creates the Mandelbrot Fractal Definition */
	OP_Operator* mandelbrot = new OP_Operator(
		"cc::fractal_mandelbrot", // Node Name
		"CC Fractal Mandelbrot", // Pretty Name
		&CC::OP::get_new_op<CC::COP2_Mandelbrot>,
		&CC::COP2_Mandelbrot::myTemplatePair,
		0,  // min inputs
		0,  // max inputs
		&CC::COP2_Mandelbrot::myVariablePair,
		OP_FLAG_GENERATOR);

	/** Creates the Pickover Definition */
	OP_Operator* pickover = new OP_Operator(
		"cc::fractal_pickover", // Node Name
		"CC Fractal Pickover", // Pretty Name
		&CC::OP::get_new_op<CC::COP2_Pickover>,
		&CC::COP2_Pickover::myTemplatePair,
		0,  // min inputs
		0,  // max inputs
		&CC::COP2_Pickover::myVariablePair,
		OP_FLAG_GENERATOR);

	std::vector<OP_Operator*> nodes{
		buddhabrot, fractalMatte, lyapunov, mandelbrot, pickover };

	for (auto node : nodes)
	{
		node->setOpTabSubMenuPath(CC::menuPath);
		table->addOperator(node);
	}
}