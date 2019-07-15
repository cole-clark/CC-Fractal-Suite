/*
	Cole Clark's Fractal Suite

	COP2_Lyapunov.h
	Header for CC Lyapunov Cop Node.
 */

#pragma once

#include <COP2/COP2_Generator.h>

#include "COP2_Mandelbrot.h"

static PRM_Name nameScale("scale", "Scale");
#define XFORM_PARMS \
	PRM_Template(PRM_FLT_J, TOOL_PARM, 2, &nameScale), \
	PRM_Template()

namespace CC
{
	class COP2_Lyapunov : public COP2_Generator
	{
		COP2_Lyapunov(OP_Network* parent, const char* name, OP_Operator* entry);

	public:
		static OP_Node *myConstructor(OP_Network*, const char*, OP_Operator*);

		static PRM_Template myTemplateList[];
		static OP_TemplatePair myTemplatePair;
		static OP_VariablePair myVariablePair;
		static CH_LocalVariable myVariableList[];

	protected:
		/// Creates the image. This is called by multiple worker threads by Houdini.
		OP_ERROR generateTile(COP2_Context& context, TIL_TileList* tileList) { return error(); };
	};
}