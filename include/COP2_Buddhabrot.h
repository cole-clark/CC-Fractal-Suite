/** \file COP2_Buddhabrot.h
	Header declaring the Buddhabrot Cop2 Operator.
 */

#pragma once

 // Local
#include "Mandelbrot.h"
#include "FractalNode.h"

// HDK
#include <COP2/COP2_MaskOp.h>

// STL
#include <random>

namespace CC
{
/** A low iteration value that will roughly display the fractal.*/
static const int REFERENCE_FRACTAL_ITERS{ 10 };

/**Small object storing both the Fractal and the Transformation space info.
 * This is necessary because its values are copied to each tile, so the data
 * within can be sourced a single time, but accessed many times across multiple
 * threads without forcing const types.
 * Also stores the lock used by the full-image filter cook*/
struct COP2_BuddhabrotData : public COP2_ContextData
{
	Mandelbrot fractal;
	FractalSpace space;
	UT_Lock myLock;
	int seed;
	fpreal samples;
	bool normalize;
	int maxval;
	bool displayreffractal;

	COP2_BuddhabrotData() = default;
	virtual ~COP2_BuddhabrotData() = default;

};

/**Mandelbrot Operator class. Inherits from COP2_Maskop, meaning it will
 * the entire image at once as a 'full-image filter'. Unlike other nodes
 * in the CCFS, this node is not a pure generator, and requires an input
 * node. See 'COP Concepts' in the HDK documentation.*/
class COP2_Buddhabrot : public COP2_MaskOp
{
public:

	/** Assign Template Pair of node to generator.*/
	static OP_TemplatePair myTemplatePair;

	/** Assign empty variable pairing.*/
	static OP_VariablePair myVariablePair;

	/**Populated heavily from macros from FractalNode.h*/
	static PRM_Template myTemplateList[];

	/**Empty.*
	static CH_LocalVariable myVariableList[];

	/**Name the labels for the inputs.*/
	static const char* myInputLabels[];

	/**Static cookFullImage callback method.*/
	static OP_ERROR filter(
		COP2_Context& context,
		const TIL_Region* input,
		TIL_Region* output,
		COP2_Node* me);

	/** Non-static cookFullImage callback, called by filter.*/
	OP_ERROR filterImage(
		COP2_Context& context,
		const TIL_Region* input,
		TIL_Region* output);

	/** Specifies thread usage for this node.*/
	virtual void getMaxNumThreadsInCook(
		COP2_Context&,
		int& maxp, int& maxn, int& op) const
	{
		// MAX Plane cooking threads
		maxp = 1;
		// MAX Node cooking threads
		maxn = op = TIL_MAX_THREADS;
	}

	/**Used for fetching incoming COP2 image data.*/
	virtual void getInputDependenciesForOutputArea(
		COP2_CookAreaInfo& output_area,
		const COP2_CookAreaList& input_areas,
		COP2_CookAreaList& needed_areas);

	/** Calculates Set of Fractal Set Coords Based on fractalCoords Position*/
	std::vector<COMPLEX> buddhabrotPoints(
		Mandelbrot* fractal,
		const COMPLEX& c,
		unsigned int nIterations);

	/**Accessor used to construct this object in register.cpp*/
	friend class OP;

protected:
	virtual ~COP2_Buddhabrot();

	/** Evaluate Parms and Stash Data for Cooking in a contextData object.*/
	virtual COP2_ContextData* newContextData(const TIL_Plane*, int,
		fpreal32 t, int xres, int yres, int thread, int max_threads);

	/** Generates the image. This is a single-threaded call. */
	virtual OP_ERROR doCookMyTile(
		COP2_Context& context,
		TIL_TileList* tiles);

	/** Sets the image bounds.
	While not virtual, removing this method results in a black frame.*/
	virtual void computeImageBounds(COP2_Context &context);

	/** Use to hide/unhide parameters.*/
	virtual bool updateParmsFlags() override;

private:

	/** Private constructor, only accessed through the OP friend class. */
	COP2_Buddhabrot(
		OP_Network* parent,
		const char* name,
		OP_Operator* entry);

	/** Creates the Buddhabrot, and sets values For image idata and odata,
	 * whichcalls to the inputs and outputs of a TIL_Region.
	 * Returns highest sampled value. */
	int COP2_Buddhabrot::evaluateBuddhabrot(
		COP2_BuddhabrotData* sdata,
		const COP2_Context& context,
		char* idata,
		char* odata,
		std::mt19937& rng,
		const int numSamples);

	/** Normalizes the buddhabrot fractal based on either a user-defined
	 * maximum, or the highest Value sampled by the Buddhabrot. */
	void COP2_Buddhabrot::normalizeBuddhabrot(
		COP2_BuddhabrotData* sdata,
		const COP2_Context& context,
		char* odata,
		int highest_sample_value);

	/** Display a Mandelbrot fractal to guide the user.
	 * Note that the input masking is taken into account. */
	void COP2_Buddhabrot::displayReferenceFractal(
		COP2_BuddhabrotData* sdata,
		const COP2_Context& context,
		char* idata,
		char* odata,
		Mandelbrot& refFractal);
};
}