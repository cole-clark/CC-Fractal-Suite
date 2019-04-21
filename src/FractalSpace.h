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


namespace CC
{
	std::pair<int, int> calculate_world_pixel(
		TIL_TileList* tiles,
		TIL_Tile* tile,
		int pixel_index);


	class FractalSpace
	{
		int image_x{ 0 };
		int image_y{ 0 };
		UT_Matrix3T<float> post_matrix;
		UT_XformOrder::rstOrder xform_order{ UT_XformOrder::STR };  // Scale Translate Rotate


	public:

		FractalSpace();
		FractalSpace(int x, int y);

		// TODO : Implement
		FCOORDS get_fractal_coords(std::pair<int, int>pixel_coords);

		void set_image_size(int x, int y);

		// TODO : Implement
		void set_xform(float scale, UT_Vector2T<float> translate, float rotate);


		virtual ~FractalSpace();

	};
}