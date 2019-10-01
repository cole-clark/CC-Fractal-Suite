/** \file register.h
	Header for source declares new(Context)Operator functions.

 * Houdini automatically calls these methods to add our Operators to the
 * operator table. Therefore, these are kept in the global namespace as the
 * usage of these functions do not belong to us.
 * The newCop2Operator method cannot have its header re-declared here,
 * so it only exists in source.
 */

#pragma once

namespace CC
{
/** Declares the Tab Op Menu Path for Nodes in the CCFS. */
static UT_String menuPath{ "Fractal" };

}  // End of CC Namespace