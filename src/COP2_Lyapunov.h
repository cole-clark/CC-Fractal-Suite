/*
	Cole Clark's Fractal Suite

	COP2_Lyapunov.h
	Header for CC Lyapunov Cop Node.
 */

#pragma once

#include <COP2/COP2_Generator.h>

#include "Lyapunov.h"
#include "FractalSpace.h"

namespace CC
{
	class COP2_Lyapunov : public COP2_Generator
	{
		COP2_Lyapunov(OP_Network* parent, const char* name, OP_Operator* entry);

	public:

		static PRM_Template myTemplateList[];
		static OP_TemplatePair myTemplatePair;
		static OP_VariablePair myVariablePair;
		static CH_LocalVariable myVariableList[];

		/// Determine Frame Range, Image Composition, and other Sequence Info
		virtual TIL_Sequence* cookSequenceInfo(OP_ERROR& error);

		friend class OP;

	protected:
		/// Evaluate Parms and Stash Data for Cooking In a COP2_ContextData object
		virtual COP2_ContextData* newContextData(const TIL_Plane*, int,
			float t, int xres, int yres, int thread, int max_threads);

		/// Creates the image. This is called by multiple worker threads by Houdini.
		OP_ERROR generateTile(COP2_Context& context, TIL_TileList* tileList) { return error(); };
	};

	struct COP2_LyapunovData : public COP2_ContextData
	{
		FractalSpace space;
		Lyapunov fractal;

		COP2_LyapunovData() = default;
		virtual ~COP2_LyapunovData();
	};
}