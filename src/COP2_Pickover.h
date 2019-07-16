/*
	Cole Clark's Fractal Suite

	COP2_Pickover.h
	Header for CC Lyapunov Cop Node.
 */

#pragma once

#include <COP2/COP2_Generator.h>

#include "FractalSpace.h"
#include "Mandelbrot.h"
#include "FractalNode.h"


namespace CC
{
	/// Public Import the COP2_Generator base class
	class COP2_Pickover : public COP2_Generator
	{
		COP2_Pickover(OP_Network* parent, const char* name, OP_Operator* entry);

	public:
		/// Static Lists to define parameters and local variables
		static PRM_Template myTemplateList[];
		static OP_TemplatePair myTemplatePair;
		static OP_VariablePair myVariablePair;
		static CH_LocalVariable myVariableList[];

		/// Determine Frame Range, Image Composition, and other Sequence Info
		virtual TIL_Sequence* cookSequenceInfo(OP_ERROR& error);

		friend class OP;
		//friend class XformStashData;

	protected:
		/// Evaluate Parms and Stash Data for Cooking In a COP2_ContextData object
		virtual COP2_ContextData* newContextData(const TIL_Plane*, int,
			float t, int xres, int yres, int thread, int max_threads);

		/// Create the image data for a single tile list -- multithreaded call
		virtual OP_ERROR generateTile(COP2_Context& context, TIL_TileList* tilelist);

		/// Destructor
		virtual ~COP2_Pickover();
	};

	/// This class is used to stash the evaluated parms and data.
	/// The data here is fed to several worker threads when cooking the node.
	struct COP2_PickoverData : public COP2_ContextData
	{
		FractalSpace space;
		Mandelbrot fractal;

		COP2_PickoverData() = default;
		virtual ~COP2_PickoverData();
	};
}