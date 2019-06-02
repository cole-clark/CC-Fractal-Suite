/*
	Cole Clark's Fractal Suite

	COP2_FractalMatte.h
	Header for CC Fractal Matte Cop filter node.
 */

#pragma once

#include <COP2/COP2_PixelOp.h>
#include <RU/RU_PixelFunctions.h>

namespace CC
{
	class COP2_FractalMatte : public COP2_PixelOp
	{
	public:
		static OP_Node* myConstructor(OP_Network*, const char*, OP_Operator*);

		static OP_TemplatePair myTemplatePair;
		static OP_VariablePair myVariablePair;
		static PRM_Template myTemplateList[];
		static CH_LocalVariable myVariableList[];
		static const char* myInputLabels[];

	protected:
		/// We must override this method, which returns a pixel function.
		virtual RU_PixelFunction* addPixelFunction(
			const TIL_Plane*,
			int,
			float t,
			int,
			int,
			int thread);

	private:
		COP2_FractalMatte(
			OP_Network* parent,
			const char* name,
			OP_Operator* entry);

		virtual ~COP2_FractalMatte();

		/// Optional description for OP info popup.
		virtual const char* getInfoPopup();
	};

	class cop2_FractalMatteFunc : public RU_PixelFunction
	{
	public:
		cop2_FractalMatteFunc(int modulo, int offset, bool invert=false);

	protected:
		virtual bool eachComponentDifferent() const
		{
			return false;
		}

		virtual bool needAllComponents() const
		{
			return false;
		}

		/// This is the core function that does the actual work.
		static float checkModulus(
			RU_PixelFunction* pf,  // Pointer to pixel function
			// Float values are assumed for pixel functions, though we will cast to int.
			float pixelValue,
			// Related to which component of the modulus val would apply, if we were using
			// more than one component.
			int comp);

		/// This is how we signal to RU_PixelFunction what method must be called
		/// per-pixel
		virtual RUPixelFunc getPixelFunction() const
		{
			return checkModulus;
		}

	private:
		int modulo{ 0 };
		int offset{ 0 };
		bool invert{ false };
	};
}