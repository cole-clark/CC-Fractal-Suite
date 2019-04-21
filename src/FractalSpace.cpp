/*
	Cole Clark's Fractal Suite

	FractalSpace.h
	Code related to everything related to Xforms in Fractal Space.
 */

#include <sys/SYS_Math.h>

#include "FractalSpace.h"

/// Calculates the 'tile origin', or the pixel number for where the tile 's first pixel is.
/// This is used as a reference point for the world pixel.
std::pair<int, int> CC::calculate_world_pixel(TIL_TileList* tiles, TIL_Tile* tile, int pixel_index)
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
	y = SYSfloor((float)pixel_index / tile_x) + (tiles->myY1);
	return std::pair<int, int>(x, y);
}

CC::FractalSpace::FractalSpace()
{
	post_matrix.identity();
}

/// A fractal space must be populated with a default image size.
/// The image size is used as a rest size for the fractal, which is normalized by
/// The size of the image. This is done so that changes in resolution don't require
/// Changes in transformation to see the same Fractal.
CC::FractalSpace::FractalSpace(int image_x, int image_y) :
	image_x(image_x),
	image_y(image_y) 
{
	//post_matrix->identity();  // Initialize identity matrix
}


/// Return the fractal coordinates, which use the size of the image as a relative
/// Size. The scale is 0-1 in the x axis of the image.
std::pair<double, double> 
CC::FractalSpace::get_fractal_coords(std::pair<int, int>pixel_coords)
{
	return std::pair<double, double>();
}

void CC::FractalSpace::set_image_size(int x, int y)
{
	image_x = x;
	image_y = y;
}

/// Destructor
CC::FractalSpace::~FractalSpace() {}
