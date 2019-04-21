/*
	Cole Clark's Fractal Suite

	FractalSpace.h
	Header defining everything related to Xforms in Fractal Space.
 */

#pragma once

#include <TIL/TIL_Tile.h>
#include <TIL/TIL_TileList.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_XformOrder.h>

#include <utility>

typedef std::pair<double, double> FCOORDS;
typedef std::pair<int, int> WORLDPIXELCOORDS;
typedef UT_XformOrder::rstOrder RSTORDER;

namespace CC
{
	///Get the Houdini rstOrder enum value from the interface
	RSTORDER get_rst_order(const int val);

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
			const float tx,
			const float ty,
			const float r,
			const float sx,
			const float sy,
			const float pivx,
			const float pivy,
			const RSTORDER xord);

		// TODO : Implement
		FCOORDS get_fractal_coords(WORLDPIXELCOORDS pixel_coords);

		void set_image_size(int x, int y);

		virtual ~FractalSpace();

	};
}