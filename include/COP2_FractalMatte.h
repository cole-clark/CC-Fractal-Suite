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

		static OP_TemplatePair myTemplatePair;
		static OP_VariablePair myVariablePair;
		static PRM_Template myTemplateList[];
		static CH_LocalVariable myVariableList[];
		static const char* myInputLabels[];

		friend class OP;

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

	};

	class cop2_FractalMatteFunc : public RU_PixelFunction
	{
	public:

		enum class ModeType
		{
			MODULUS,
			COMPARISON,
			BLENDCOLOR
		};

		enum class ComparisonType
		{
			LESS_THAN,
			LESS_THAN_EQUALS,
			EQUALS,
			GREATER_THAN_EQUALS,
			GREATER_THAN,
			NOT_EQUALS
		};

		enum class BlendType
		{
			LINEAR,
			QUADRATIC,
			CONSTANT
		};

		// For Modulus
		cop2_FractalMatteFunc(
			double modulo, double offset, bool invert=false);

		// For Comparison
		cop2_FractalMatteFunc(
			double compValue, ComparisonType compType, bool invert = false);

		// For Color Blending
		cop2_FractalMatteFunc(
			std::vector<double>sizes,
			std::vector<UT_Color>colors,
			BlendType blendType,
			fpreal32 offset,
			fpreal32 blendOffset,
			fpreal32 weightMult = 1.0f,
			bool invert = false);
	protected:
		virtual bool eachComponentDifferent() const
		{
			if (mode == ModeType::BLENDCOLOR)
				return true;
			// Unless a BLENDCOLOR, all components can be the same.
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

		/// Calculate Blended Colors based on a vector of colors and weights.
		static float checkBlendColors(
			RU_PixelFunction* pf,
			float pixelValue,
			int comp);

		/// This is how we signal to RU_PixelFunction what method must be called
		/// per-pixel.
		/// Returns a different function depending on what mode type is used.
		virtual RUPixelFunc getPixelFunction() const
		{
			switch (mode)
			{
			case ModeType::MODULUS:
				return checkModulus;
			case ModeType::COMPARISON:
				return checkComparison;
			case ModeType::BLENDCOLOR:
				return checkBlendColors;
			default:
				return checkModulus;
			}
		}

	private:
		fpreal32 modulo{ 0.0f };
		fpreal32 offset{ 0.0f };
		bool invert{ false };
		fpreal32 compValue{ 0.0 };
		std::vector<double> sizes;
		std::vector<UT_Color> colors;
		BlendType blendType;
		ComparisonType compType{ ComparisonType::LESS_THAN };
		ModeType mode{ ModeType::MODULUS };
		fpreal32 colorOffset{ 0.0f };
		fpreal32 blendOffset{ 1.0f };
	};
}