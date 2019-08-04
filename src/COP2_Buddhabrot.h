/*
	Cole Clark's Fractal Suite

	COP2_Buddhabrot.h
	Header for CC Buddhabrot Cop Node.
 */

#pragma once

#include <COP2/COP2_MaskOp.h>

#include "Mandelbrot.h"
#include "FractalNode.h"

// A low value that will roughly display the fractal
static const int REFERENCE_FRACTAL_ITERS{ 10 };

namespace CC
{
	class COP2_Buddhabrot : public COP2_MaskOp
	{
	public:
		static OP_TemplatePair myTemplatePair;
		static OP_VariablePair myVariablePair;
		static PRM_Template myTemplateList[];
		static CH_LocalVariable myVariableList[];
		static const char* myInputLabels[];

		/// Static cookFullImage callback method.
		static OP_ERROR filter(
			COP2_Context& context,
			const TIL_Region* input,
			TIL_Region* output,
			COP2_Node* me);

		/// Non-static cookFullImage callback, called by filter.
		OP_ERROR filterImage(
			COP2_Context& context,
			const TIL_Region* input,
			TIL_Region* output);

		/// Specifies thread usage for this node.
		virtual void getMaxNumThreadsInCook(
			COP2_Context&,
			int& maxp, int& maxn, int& op) const
		{
			// MAX Plane cooking threads
			maxp = 1;
			// MAX Node cooking threads
			maxn = op = TIL_MAX_THREADS; 
		}

		virtual void getInputDependenciesForOutputArea(
			COP2_CookAreaInfo& output_area,
			const COP2_CookAreaList& input_areas,
			COP2_CookAreaList& needed_areas);

		/// Calculates Set of Fractal Set Coords Based on Position
		std::vector<COMPLEX> buddhabrotPoints(
			Mandelbrot* fractal,
			const COMPLEX& c,
			unsigned int nIterations);

		friend class OP;

	protected:
		virtual ~COP2_Buddhabrot();
		virtual COP2_ContextData* newContextData(
			const TIL_Plane* p,
			int array_index,
			float t,
			int xres, int yres,
			int thread,
			int max_threads);

		virtual OP_ERROR doCookMyTile(
			COP2_Context& context,
			TIL_TileList* tiles);

		// Sets the image bounds. While not virtual, removing
		// this method results in a black frame.
		virtual void computeImageBounds(COP2_Context &context);

		/// Use to hide/unhide parameters.
		virtual bool updateParmsFlags() override;

	private:
		COP2_Buddhabrot(
			OP_Network* parent,
			const char* name,
			OP_Operator* entry);
	};


	struct COP2_BuddhabrotData : public COP2_ContextData
	{
		Mandelbrot fractal;
		FractalSpace space;
		UT_Lock myLock;
		int seed;
		double samples;
		bool normalize;
		int maxval;
		bool displayreffractal;

		COP2_BuddhabrotData() = default;
		virtual ~COP2_BuddhabrotData() = default;

	};
}