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

		/// Use to hide/unhide parameters.
		virtual bool updateParmsFlags() override;

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

		enum class ModeType
		{
			MODULUS,
			COMPARISON
		};

		enum class ComparisonType
		{
			LESS_THAN,
			LESS_THAN_EQUALS,
			EQUALS,
			GREATER_THAN_EQUALS,
			GREATER_THAN
		};

		cop2_FractalMatteFunc(
			int modulo, int offset, bool invert=false);

		cop2_FractalMatteFunc(
			int compValue, ComparisonType compType, bool invert = false);

	protected:
		virtual bool eachComponentDifferent() const
		{
			return false;
		}

		virtual bool needAllComponents() const
		{
			return false;
		}

		/// Calculate Pixel Matte from a modulus
		static float checkModulus(
			RU_PixelFunction* pf,  // Pointer to pixel function
			// Float values are assumed for pixel functions, though we will cast to int.
			float pixelValue,
			// Related to which component of the modulus val would apply, if we were using
			// more than one component.
			int comp);

		/// Calculate Pixel Matte from a simple operator comparison/
		static float checkComparison(
			RU_PixelFunction* pf,
			float pixelValue,
			int comp);

		/// This is how we signal to RU_PixelFunction what method must be called
		/// per-pixel.
		/// Returns a different function depending on what mode type is used.
		virtual RUPixelFunc getPixelFunction() const
		{
			if (mode == ModeType::MODULUS)
				return checkModulus;
			else if (mode == ModeType::COMPARISON)
				return checkComparison;
		}

	private:
		int modulo{ 0 };
		int offset{ 0 };
		bool invert{ false };
		int compValue{ 0 };
		ComparisonType compType{ ComparisonType::LESS_THAN };
		ModeType mode{ ModeType::MODULUS };
	};
}