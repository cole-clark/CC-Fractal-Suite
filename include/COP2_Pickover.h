/** \file COP2_Pickover.h
	Header declaring the Pickover Cop2 Operator.
 */

#pragma once

 // Local
#include "FractalSpace.h"
#include "Mandelbrot.h"
#include "FractalNode.h"

// HDK
#include <COP2/COP2_Generator.h>


namespace CC
{
/**Pickover Operator class. Inherits from COP2_Generator, meaning it will cook
 * in tiles. See 'COP Concepts' in the HDK documentation.*/
class COP2_Pickover : public COP2_Generator
{
	/** Private constructor, only accessed through the OP friend class. */
	COP2_Pickover(OP_Network* parent, const char* name, OP_Operator* entry);

public:
	/**Populated heavily from macros from FractalNode.h*/
	static PRM_Template myTemplateList[];

	/** Assign Template Pair of node to generator.*/
	static OP_TemplatePair myTemplatePair;

	/** Assign empty variable pairing.*/
	static OP_VariablePair myVariablePair;

	/**Empty.*/
	static CH_LocalVariable myVariableList[];

	/** Determine Frame Range, Image Composition, and other Sequence Info */
	virtual TIL_Sequence* cookSequenceInfo(OP_ERROR& error);

	/**Accessor used to construct this object in register.cpp*/
	friend class OP;

protected:
	/** Evaluate Parms and Stash Data for Cooking in a contextData object. */
	virtual COP2_ContextData* newContextData(const TIL_Plane*, int,
		fpreal32 t, int xres, int yres, int thread, int max_threads);

	/** Generates the image. This is a multi-threaded call. */
	virtual OP_ERROR generateTile(
		COP2_Context& context, TIL_TileList* tilelist);

	/** Used to hide/unhide parameters. */
	virtual bool updateParmsFlags() override;

	virtual ~COP2_Pickover();
};

/**Small object storing both the Fractal and the Transformation space info.
 * This is necessary because its values are copied to each tile, so the data
 * within can be sourced a single time, but accessed many times across multiple
 * threads without forcing const types. */
struct COP2_PickoverData : public COP2_ContextData
{
	FractalSpace space;
	Pickover fractal;

	/**Calculates the reference fractal.*/
	fpreal32 calculate_reference(
		COMPLEX fractalCoords, WORLDPIXELCOORDS worldPixel);

	/** The pixel-space location of the pickover point position.*/
	WORLDPIXELCOORDS world_point;

	COP2_PickoverData() = default;
	virtual ~COP2_PickoverData();
};
}