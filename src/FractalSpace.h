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
#include <vector>
#include <initializer_list>

#include "typedefs.h"
#include "StashData.h"


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
		UT_Matrix3D post_matrix;
		XformStashData xdata;

	public:

		FractalSpace();
		FractalSpace(int x, int y);

		void set_xform(MultiXformStashData& xdata);
		void set_xform(XformStashData& xdata);
		void set_xform(UT_Matrix3D& xform);

		COMPLEX get_fractal_coords(WORLDPIXELCOORDS pixel_coords);
		COMPLEX get_fractal_coords(COMPLEX pixel_coords);
		WORLDPIXELCOORDS get_pixel_coords(COMPLEX fractal_coords);

		///Returns in fractal coords the min/max of the image.
		COMPLEX get_minimum();
		COMPLEX get_maximum();

		void set_image_size(int x, int y);
		WORLDPIXELCOORDS get_image_size();

		virtual ~FractalSpace();

	};
}