/** \file COP2_FractalMatte.h
	Header declaring the Fractal Matte Cop2 Filter Operator.
 */

#pragma once

 // HDK
#include <COP2/COP2_PixelOp.h>
#include <RU/RU_PixelFunctions.h>

namespace CC
{

/** Fractal Matte Operator class.
 * Inherits from COP2_PixelOp, which means that this cooks as a
 * 'pixel operator', meaning that pixels don't have an awareness of each other
 * like with tiles, kernels, or fill-image filters.
 * See 'COP Concepts' in the HDK documentation.
 */
class COP2_FractalMatte : public COP2_PixelOp
{
public:

	static OP_TemplatePair myTemplatePair;
	static OP_VariablePair myVariablePair;
	static PRM_Template myTemplateList[];
	static CH_LocalVariable myVariableList[];

	/**Label the names of the inputs in the Houdini UI*/
	static const char* myInputLabels[];

	/**Accessor used to construct this object in register.cpp*/
	friend class OP;

protected:
	/** We must override this method, which returns a pixel function. */
	virtual RU_PixelFunction* addPixelFunction(
		const TIL_Plane*,
		int,
		fpreal32 t,
		int,
		int,
		int thread);

	/** Use to hide/unhide parameters.*/
	virtual bool updateParmsFlags() override;

private:

	/** Private constructor, only accessed through the OP friend class. */
	COP2_FractalMatte(
		OP_Network* parent,
		const char* name,
		OP_Operator* entry);

	virtual ~COP2_FractalMatte();

};

/**Pixel Function used to interpret the upstream fractals.
 * Supports various modes that connect the interface to what the pixel
 * values should be.
 * This may get refactored in the future to distinct functions, but since
 * only one is being run on the image at a time, and there isn't a performance
 * benefit, I have kept them altogether for now.*/
class cop2_FractalMatteFunc : public RU_PixelFunction
{
public:

	/**Enumerates the different type of fundamental operators we can perform
	 * on the upstream fractals.*/
	enum class ModeType
	{
		MODULUS,
		COMPARISON,
		BLENDCOLOR
	};

	/**When in the comparison mode, specifies the different kinds of
	 * comparisons we are allowed to make. These correspond directly to
	 * C++'s relational operators. */
	enum class ComparisonType
	{
		LESS_THAN,
		LESS_THAN_EQUALS,
		EQUALS,
		GREATER_THAN_EQUALS,
		GREATER_THAN,
		NOT_EQUALS
	};

	/**When in the blendcolor mode, specifies the strategy used to blend
	 * the colors between different value ranges.*/
	enum class BlendType
	{
		LINEAR,
		QUADRATIC,
		CONSTANT
	};

	/** For Modulus*/
	cop2_FractalMatteFunc(
		fpreal modulo, fpreal offset, bool invert = false);

	/** For Comparison*/
	cop2_FractalMatteFunc(
		fpreal compValue, ComparisonType compType, bool invert = false);

	/** For Color Blending*/
	cop2_FractalMatteFunc(
		std::vector<fpreal>sizes,
		std::vector<UT_Color>colors,
		BlendType blendType,
		fpreal32 offset,
		fpreal32 blendOffset,
		fpreal32 weightMult = 1.0f,  // Zero values here are really bad
		bool invert = false);
protected:
	/**Tells Houdini whether 'component', here meaning image plane,
	 * should be different or not.*/
	virtual bool eachComponentDifferent() const
	{
		if (mode == ModeType::BLENDCOLOR)
			return true;
		// Unless a BLENDCOLOR, all components can be the same.
		return false;
	}

	/**Tells Houdini if all components must be cooked or not*/
	virtual bool needAllComponents() const
	{
		return false;
	}

	/** Calculate Pixel Matte from a modulus*/
	static fpreal32 checkModulus(
		RU_PixelFunction* pf,  /**> Pointer to pixel function*/
		/**> Pixel functions recieve float values, but we will cast to int. */
		fpreal32 pixelValue,
		/**> Which image plane we are currently calculating*/
		int comp);

	/** Calculate Pixel Matte from a simple operator comparison*/
	static fpreal32 checkComparison(
		RU_PixelFunction* pf,
		fpreal32 pixelValue,
		int comp);

	/** Calculate Blended Colors based on a vector of colors and weights.*/
	static fpreal32 checkBlendColors(
		RU_PixelFunction* pf,
		fpreal32 pixelValue,
		int comp);

	/** This is how we signal to RU_PixelFunction what method must be called
	 * per-pixel.
	 * Returns a different function depending on what mode type is used. */
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
	std::vector<fpreal> sizes;
	std::vector<UT_Color> colors;
	BlendType blendType;
	ComparisonType compType{ ComparisonType::LESS_THAN };
	ModeType mode{ ModeType::MODULUS };
	fpreal32 colorOffset{ 0.0f };
	fpreal32 blendOffset{ 1.0f };
};
}