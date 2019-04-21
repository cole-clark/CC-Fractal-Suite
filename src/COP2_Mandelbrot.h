/*
	Cole Clark's Fractal Suite

	COP2_Mandelbrot.h
	Header for CC Mandelbrot Cop Node.
 */

#pragma once

#include <COP2/COP2_Generator.h>

#include "FractalSpace.h"
#include "Mandelbrot.h"

namespace CC
{
	/// Public Import the COP2_Generator base class
	class COP2_Mandelbrot : public COP2_Generator
	{
		COP2_Mandelbrot(OP_Network* parent, const char* name, OP_Operator* entry);

	public:
		/// Static Public Constructor, check register.h for how it's used.
		static OP_Node *myConstructor(OP_Network*, const char*, OP_Operator*);

		/// Static Lists to define parameters and local variables
		static PRM_Template myTemplateList[];
		static OP_TemplatePair myTemplatePair;
		static OP_VariablePair myVariablePair;
		static CH_LocalVariable myVariableList[];

		/// Determine Frame Range, Image Composition, and other Sequence Info
		virtual TIL_Sequence* cookSequenceInfo(OP_ERROR& error);

	protected:
		/// Evaluate Parms and Stash Data for Cooking In a COP2_ContextData object
		virtual COP2_ContextData* newContextData(const TIL_Plane*, int,
			float t, int xres, int yres, int thread, int max_threads);

		/// Create the image data for a single tile list -- multithreaded call
		virtual OP_ERROR generateTile(COP2_Context& context, TIL_TileList* tilelist);

		/// Destructor
		virtual ~COP2_Mandelbrot();
	};

	/// This class is used to stash the evaluated parms and data.
	/// The data here is fed to several worker threads when cooking the node.
	struct COP2_MandelbrotData : public COP2_ContextData
	{
		FractalSpace space;
		Mandelbrot fractal;

		COP2_MandelbrotData() = default;
		virtual ~COP2_MandelbrotData();
	};
}
