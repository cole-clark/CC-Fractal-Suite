/*
	Cole Clark's Fractal Suite

	COP2_Buddhabrot.h
	Header for CC Buddhabrot Cop Node.
 */

#pragma once

#include <COP2/COP2_Generator.h>

#include "FractalSpace.h"
#include "Mandelbrot.h"

namespace CC
{
	/// Public Import the COP2_Generator base class
	class COP2_Buddhabrot : public COP2_Generator
	{
		COP2_Buddhabrot(OP_Network* parent, const char* name, OP_Operator* entry);

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

		/// Return Buddhabrot-friendly points based on the fed fractal and sample position.
		std::vector<COMPLEX> buddhabrotPoints(Mandelbrot* fractal, const COMPLEX& c, int numIters);

	protected:
		/// Evaluate Parms and Stash Data for Cooking In a COP2_ContextData object
		virtual COP2_ContextData* newContextData(const TIL_Plane*, int,
			float t, int xres, int yres, int thread, int max_threads);

		/// Create the image data for a single tile list -- multithreaded call
	// static cookFullImage callback.
		static OP_ERROR     generateStatic(COP2_Context &context,
			const TIL_Region *input,
			TIL_Region *output,
			COP2_Node  *me);
		// non static version, called by filter.
		OP_ERROR generate(COP2_Context &context,
			TIL_Region *output);

		virtual OP_ERROR generateTile(COP2_Context& context, TIL_TileList* tilelist);

		virtual OP_ERROR    doCookMyTile(COP2_Context &context,
			TIL_TileList *tiles);

		virtual void getMaxNumThreadsInCook(COP2_Context &,
			int &maxp, int &maxn, int &op) const
		{
			maxp = 1; maxn = op = TIL_MAX_THREADS;
		}

		/// Destructor
		virtual ~COP2_Buddhabrot();
	};

	/// This class is used to stash the evaluated parms and data.
	/// The data here is fed to several worker threads when cooking the node.
	struct COP2_BuddhabrotData : public COP2_ContextData
	{
		FractalSpace space;
		Mandelbrot fractal;
		UT_Lock myLock;
		int samples;
		fpreal seed;

		COP2_BuddhabrotData() = default;
		virtual ~COP2_BuddhabrotData();
	};
}
