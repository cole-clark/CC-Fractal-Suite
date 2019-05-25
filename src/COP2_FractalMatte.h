/*
	Cole Clark's Fractal Suite

	COP2_FractalMatte.h
	Header for CC Fractal Matte Cop filter node.
 */

class RU_PixelFunction;
#include <COP2/COP2_PixelOp.h>
namespace CC {
	class COP2_FractalMatte : public COP2_PixelOp
	{
	public:

		static OP_Node              *myConstructor(OP_Network*, const char *,
			OP_Operator *);
		static OP_TemplatePair       myTemplatePair;
		static OP_VariablePair       myVariablePair;
		static PRM_Template          myTemplateList[];
		static CH_LocalVariable      myVariableList[];
		static const char *          myInputLabels[];
	protected:
		/// This is the only function we need to override for a pixel function.
		/// It returns our pixel function, which must be derived from
		/// RU_PixelFunction.
		virtual RU_PixelFunction    *addPixelFunction(const TIL_Plane *, int,
			float t, int, int,
			int thread);

	private:
		COP2_FractalMatte(OP_Network *parent, const char *name,
			OP_Operator *entry);
		virtual     ~COP2_FractalMatte();

		/// An optional method which returns a short description of what this node
		/// does in the OP info popup.
		virtual const char  *getOperationInfo();
		fpreal      ADD(int comp, fpreal t)
		{
			return evalFloat("addend", comp, t);
		}
	};
} // End HDK_Sample namespace