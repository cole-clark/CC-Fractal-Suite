/*
	Cole Clark's Fractal Suite

	FractalNode.h
	Header defining the basics of a fractal houdini node.
 */

#pragma once

#include <PRM/PRM_Include.h>
#include <PRM/PRM_ChoiceList.h>
#include <OP/OP_Network.h>
#include <TIL/TIL_Plane.h>
#include <COP2/COP2_Context.h>

namespace CC
{
	struct FractalXformParms
	{
		/// ChoiceList Lists
		static PRM_Name xordMenuNames[];
		static PRM_ChoiceList xOrdMenu;

		/// Parm names
		static PRM_Name nameScale;
		static PRM_Name nameOffset;
		static PRM_Name nameRotate;
		static PRM_Name nameXOrd;

		/// Parm Defaults
		static PRM_Default defaultScale;
		static PRM_Default defaultOffset[];
		static PRM_Default defaultRotate;
		static PRM_Default defaultXOrd;

		static PRM_Range rangeScale;
		static PRM_Range rangeOffset;
		static PRM_Range rangeRotate;

		FractalXformParms();
		virtual ~FractalXformParms();
	};

	class FractalNode
	{
	protected:
		FractalNode(OP_Network* parent, const char* name, OP_Operator* entry);
		virtual ~FractalNode();

	public:
		FractalXformParms xparms;

		static OP_Node *myConstructor(OP_Network*, const char*, OP_Operator*);

		/// Static Lists to define parameters and local variables
		static PRM_Template myTemplateList[];
		static OP_TemplatePair myTemplatePair;
		static OP_VariablePair myVariablePair;
		static CH_LocalVariable myVariableList[];

		// Adding the newContextData as a pure virtual
		virtual COP2_ContextData* newContextData(const TIL_Plane*, int,
			float t, int xres, int yres, int thread, int max_threads) = 0;


	};
}