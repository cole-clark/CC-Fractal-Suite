/*
	Cole Clark's Fractal Suite

	COP2_FractalMatte.h
	Header for CC Fractal Matte Cop filter node.
 */

#pragma once

#include <COP2/COP2_MaskOp.h>

namespace CC
{
	class COP2_FractalMatte : COP2_MaskOp
	{
	public:
		/// Node created by myConstructor method.
		static OP_Node* myConstructor(
			OP_Network*, const char*, OP_Operator*);

		/// Interface
		static OP_TemplatePair myTemplatePair;
		static OP_VariablePair myVariablePair;
		static PRM_Template myTemplateList[];
		static CH_LocalVariable myVariableList[];
		static const char* myInputLabels[];

		/// Given an area to cook, indicate what parts of an image's input
		/// are needed.
		virtual void getInputDependenciesForOutputArea
		(
			COP2_CookAreaInfo& output_area,
			const COP2_CookAreaList& input_areas,
			COP2_CookAreaList& needed_areas
		);

	protected:

		/// Expands the canvas bounds by 1 pixel in all directions.
		/// computeImageBounds() announces this to the cop engine.
		virtual void computeImageBounds(COP2_Context& context);

		/// Returns a new context instance with the parameters for the filter
		/// in it, to be used by the many threads on the tiles.
		virtual COP2_ContextData* newContextData(
			const TIL_Plane* p,
			int array_index,
			float t,
			int xres,
			int yres,
			int thread,
			int max_threads);

		/// COP2_MaskOp defines lots of nice mask blending operations in
		/// cookMyTile(), and defines doCookMyTile for us to override instead.
		/// This is where the actual image operation is performed.
		virtual OP_ERROR doCookMyTile(
			COP2_Context& context, TIL_TileList* tileList);

		/// Returns a description of the operation for the node info popup.
		virtual const char* getOperationInfo();

		virtual ~COP2_FractalMatte();

	private:
		/// Constructor
		COP2_FractalMatte(
			OP_Network* parent, const char* name, OP_Operator* entry);
	};

	struct COP2_FractalMatteData : public COP2_ContextData
	{
		COP2_FractalMatteData();

		virtual ~COP2_FractalMatteData();


	};
};