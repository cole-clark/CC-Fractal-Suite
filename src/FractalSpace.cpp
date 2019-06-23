/*
	Cole Clark's Fractal Suite

	FractalSpace.h
	Code related to everything related to Xforms in Fractal Space.
 */

#include <sys/SYS_Math.h>

#include "FractalSpace.h"

RSTORDER CC::get_rst_order(const int val)
{
	auto order = RSTORDER();

	// Houdini defines no more than 6 RST orders.
	if (val <= 5)
		order = static_cast<RSTORDER>(val);
	return order;
}

/// Calculates the minimum and maximum pixel positions for a given tile.
void CC::calculate_tile_minmax(
	TIL_Tile* tile,
	WORLDPIXELCOORDS& min,
	WORLDPIXELCOORDS& max)
{
	int offset_x, offset_y;
	tile->getOffset(offset_x, offset_y);

	int size_x, size_y;
	tile->getSize(size_x, size_y);

	min = WORLDPIXELCOORDS(offset_x, offset_y);

	max = WORLDPIXELCOORDS(
		offset_x + size_x - 1,
		offset_y + size_y - 1);

}

/// Calculates the 'tile origin', or the pixel number for where the tile 's first pixel is.
/// This is used as a reference point for the world pixel.
WORLDPIXELCOORDS CC::calculate_world_pixel(TIL_TileList* tiles, TIL_Tile* tile, int pixel_index)
{
	int tile_x, tile_y;
	// Note that these tile->getTileSizeX() sizes are generic,
	// while sizes from getSize() are correct
	tile->getSize(tile_x, tile_y);

	int x, y;
	// Base X pos for the tile, plus the X progression of the plane.
	x = tiles->myX1 + (pixel_index % tile_x);

	// Divide the pixel index into the size of x rounded down
	// For the tile's y, and add the base Y pos.
	y = SYSfloor((double)pixel_index / tile_x) + (tiles->myY1);
	return std::pair<int, int>(x, y);
}

/// Set Identity Matrix to matrix on construction.
CC::FractalSpace::FractalSpace()
{
	post_matrix.identity();
}

CC::FractalSpace::FractalSpace(int x, int y)
{
	set_image_size(x, y);
}

/// Constructs the matrix. from fed parameters
void CC::FractalSpace::set_xform(
	double tx,
	double ty,
	double r,
	double sx,
	double sy,
	double r_pivx,
	double r_pivy,
	double s_pivx,
	double s_pivy,
	RSTORDER xord = RSTORDER::TRS)
{
	// We are treating the parametric Y pivot relative to the size of the X axis.
	double r_image_pivy_size = r_pivy * image_y / (double)image_x;
	double s_image_pivy_size = s_pivy * image_y / (double)image_x;

	// Pre-xform only scale and translate.
	post_matrix.xform(
		xord, tx, ty, 0, sx, sy,
		s_pivx,
		s_pivy * s_image_pivy_size);

	// XForm with only rotate and rotate pivot.
	post_matrix.xform(
		xord, 0, 0, r, 1, 1,
		post_matrix(2, 0) + (sx * r_pivx),
		post_matrix(2, 1) + (sy * r_image_pivy_size));

	// Sets RST Order to be used by node.
	rstorder = xord;
}

/// Return the fractal coordinates, which use the size of the image as a relative
/// Size. The scale is 0-1 in the x axis of the image.
/// TODO: This method may not work.
COMPLEX
CC::FractalSpace::get_fractal_coords(WORLDPIXELCOORDS pixel_coords)
{
	COMPLEX fc{ pixel_coords.first / (double)image_x, pixel_coords.second / (double)image_x };
	UT_Matrix3 m;
	m.identity();
	m.xform(rstorder, fc.real(), fc.imag());

	m *= post_matrix;

	fc = { m(2, 0), m(2, 1) };  // xz, yz.

	return fc;
}

COMPLEX CC::FractalSpace::get_minimum()
{
	return get_fractal_coords(WORLDPIXELCOORDS(0, 0));
}

COMPLEX CC::FractalSpace::get_maximum()
{
	return get_fractal_coords(get_image_size());
}

/// A fractal space must be populated with a default image size to be used by the context
/// Correctly.
/// The image size is used as a rest size for the fractal, which is normalized by
/// The size of the image. This is done so that changes in resolution don't require
/// Changes in transformation to see the same Fractal.
void CC::FractalSpace::set_image_size(int x, int y)
{
	image_x = x;
	image_y = y;
}

WORLDPIXELCOORDS CC::FractalSpace::get_image_size()
{
	return WORLDPIXELCOORDS(image_x, image_y);
}

/// Destructor
CC::FractalSpace::~FractalSpace() {}
