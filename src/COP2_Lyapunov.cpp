/*
	Cole Clark's Fractal Suite

	COP2_Lyapunov.cpp
	Code for CC Lyapunov Fracatal Generator Cop Node.
 */ 

#include "COP2_Lyapunov.h"

COP_GENERATOR_SWITCHER(1, "Fractal");

using namespace CC;

/// Private Constructor
COP2_Lyapunov::COP2_Lyapunov(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry) : COP2_Generator(parent, name, entry) {}

/// Public Constructor
OP_Node *
COP2_Lyapunov::myConstructor(
	OP_Network* net,
	const char* name,
	OP_Operator* op)
{
	return new COP2_Lyapunov(net, name, op);
}

/// Assign Template Pair of node to generator
OP_TemplatePair COP2_Lyapunov::myTemplatePair
(
	COP2_Lyapunov::myTemplateList,
	&COP2_Generator::myTemplatePair
);

/// Assign empty variable pairing
OP_VariablePair COP2_Lyapunov::myVariablePair
(
	0,
	&COP2_Node::myVariablePair
);

PRM_Template
COP2_Lyapunov::myTemplateList[]
{
	PRM_Template(PRM_FLT_LOG, TOOL_PARM, 1, &nameScale),
	PRM_Template()
};