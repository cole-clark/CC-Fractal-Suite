/*
	Cole Clark's Fractal Suite

	COP2_Lyapunov.cpp
	Code for CC Lyapunov Fracatal Generator Cop Node.
 */ 

#include "COP2_Lyapunov.h"

COP_GENERATOR_SWITCHER(5, "Fractal");

using namespace CC;

/// Private Constructor
COP2_Lyapunov::COP2_Lyapunov(
	OP_Network* parent,
	const char* name,
	OP_Operator* entry) : COP2_Generator(parent, name, entry) {}

TIL_Sequence* COP2_Lyapunov::cookSequenceInfo(OP_ERROR & error)
{
	return nullptr;
}

COP2_ContextData* COP2_Lyapunov::newContextData(const TIL_Plane *, int, float t, int xres, int yres, int thread, int max_threads)
{
	return nullptr;
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

/// Create Template List
PRM_Template
COP2_Lyapunov::myTemplateList[]
{
	TEMPLATE_SWITCHER,
	TEMPLATES_XFORM,
	PRM_Template()
};
