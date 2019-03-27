#pragma once

// STANDARD
#include <utility>
#include <complex>

// HDK
#include <UT/UT_Vector2.h>
#include <COP2/COP2_Generator.h>
#include <TIL/TIL_Tile.h>

namespace CC
{
	class COP2_FractalGeneratorData;

	/// Public Import the COP2_Generator base class
	class COP2_FractalGenerator : public COP2_Generator
	{
	public:
		/// Static Constructor Declaration?
		static OP_Node *myConstructor(OP_Network*, const char*, OP_Operator*);

		/// Static Lists to define parameters and local variables
		static OP_TemplatePair myTemplatePair;
		static OP_VariablePair myVariablePair;
		static PRM_Template myTemplateList[];
		static CH_LocalVariable myVariableList[];

		/// Determine Frame Range, Image Composition, and other Sequence Info
		virtual TIL_Sequence* cookSequenceInfo(OP_ERROR& error);

		virtual float get_iterations(double x, double y, int max_iterations, fpreal mandelpow,
			fpreal mandelmax, int mode, fpreal julia_offset_x, fpreal julia_offset_y, uint pixel_id);

	protected:
		/// Evaluate Parms and Stash Data for Cooking In a COP2_ContextData object
		virtual COP2_ContextData* newContextData(const TIL_Plane*, int,
			float t, int xres, int yres, int thread, int max_threads);

		/// Create the image data for a single tile list -- multithreaded call
		virtual OP_ERROR generateTile(COP2_Context& context, TIL_TileList* tilelist);


		/// Destructor
		virtual ~COP2_FractalGenerator();

	private:
		COP2_FractalGenerator(OP_Network* parent, const char* name, OP_Operator* entry);

		/// Parameter Evaluation Methods
		int MODE(int t)
		{ 
			return evalInt("mode", 0, t); 
		}

		int MAXITER(int t)
		{
			return evalInt("maxiter", 0, t);
		}

		fpreal SCALE(fpreal t)
		{
			return evalFloat("scale", 0, t);
		}

		fpreal MANDELPOW(fpreal t)
		{
			return evalFloat("mandelbrot_pow", 0, t);
		}

		fpreal MANDELMAX(fpreal t)
		{
			return evalFloat("mandelbrot_max", 0, t);
		}

		fpreal JULIA_OFFSET_X(fpreal t)
		{
			return evalFloat("julia_offset_x", 0, t);
		}

		fpreal JULIA_OFFSET_Y(fpreal t)
		{
			return evalFloat("julia_offset_y", 0, t);
		}

		void OFFSET(UT_Vector2& off, fpreal t)
		{
			off.x() = evalFloat("offset", 0, t);
			off.y() = evalFloat("offset", 1, t);
		}
	};

	/// This class is used to hold the evaluated parms and data.
	/// The cook method generateTiles() is threaded and called in multiple places,
	/// this class caches needed data once and is used by many tiles and threads
	class COP2_FractalGeneratorData : public COP2_ContextData
	{

	public:
		UT_Vector2 offset;
		int mode;
		int maxiter;
		fpreal scale;
		fpreal mandelpow;
		fpreal mandelmax;
		fpreal julia_offset_x, julia_offset_y;

		COP2_FractalGeneratorData() : 
			offset(0.0f, 0.0f), mode(0), maxiter(1000), scale(1.0f), mandelpow(2), mandelmax(2), julia_offset_x(0), julia_offset_y(0) {}

		virtual ~COP2_FractalGeneratorData()
		{ 
			//delete fractal; 
		}

		std::pair<int, int> calculate_coords(TIL_TileList* tiles, TIL_Tile* tile, int pixel_index)
		{
			int tile_x, tile_y;
			// Note that these tile->getTileSizeX() sizes are generic, while sizes from getSize() are correct
			tile->getSize(tile_x, tile_y);

			int x, y;
			// Base X pos for the tile, plus the X progression of the plane.
			x = tiles->myX1 + (pixel_index % tile_x);

			// Divide the pixel index into the size of x rounded down
			// For the tile's y, and add the base Y pos.
			y = SYSfloor((float)pixel_index / tile_x) + (tiles->myY1);
			return std::pair<int, int>(x, y);
		}
	};
} // End of HDK_Sample namespace