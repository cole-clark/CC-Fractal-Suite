/*
	Cole Clark's Fractal Suite

	FractalSpace.h
	Header defining everything related to Xforms in Fractal Space.
 */

#pragma once

#include <TIL/TIL_Tile.h>
#include <TIL/TIL_TileList.h>
#include <UT/UT_Matrix3.h>
#include <TIL/TIL_Tile.h>

#include <utility>

#include "typedefs.h"

namespace CC
{
	///Get the Houdini rstOrder enum value from the interface
	RSTORDER get_rst_order(const int val);

	/// Calculates the minimum and maximum pixel coords for the
	/// Tile, and returns as pixel coords
	void calculate_tile_minmax(
		TIL_Tile* tile,
		WORLDPIXELCOORDS& min,
		WORLDPIXELCOORDS& max);

	WORLDPIXELCOORDS calculate_world_pixel(
		TIL_TileList* tiles,
		TIL_Tile* tile,
		int pixel_index);


	class FractalSpace
	{
		int image_x{ 0 };
		int image_y{ 0 };
		UT_Matrix3 post_matrix;
		RSTORDER rstorder{ RSTORDER::TRS };

	public:

		FractalSpace();

		void set_xform(
			double tx,
			double ty,
			double r,
			double sx,
			double sy,
			double r_pivx,
			double r_pivy,
			double s_pivx,
			double s_pivy,
			RSTORDER xord);

		COMPLEX get_fractal_coords(WORLDPIXELCOORDS pixel_coords);

		///Returns in fractal coords the min/max of the image.
		COMPLEX get_minimum();
		COMPLEX get_maximum();

		void set_image_size(int x, int y);
		WORLDPIXELCOORDS get_image_size();

		virtual ~FractalSpace();

	};
}