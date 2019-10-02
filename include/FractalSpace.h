/** \file FractalSpace.h
	Header that manages the transformations of fractals, and
	convertion from picture-plane (WORLDPIXEL) space to fractal
	space.

 */

#pragma once

 // Local
#include "typedefs.h"
#include "StashData.h"

// STL
#include <utility>
#include <vector>
#include <initializer_list>

// HDK
#include <TIL/TIL_Tile.h>
#include <TIL/TIL_TileList.h>
#include <UT/UT_Matrix3.h>
#include <TIL/TIL_Tile.h>


namespace CC
{
/**Get the Houdini rstOrder enum value from the interface.*/
RSTORDER get_rst_order(const int val);

/** Calculates the minimum and maximum pixel coords for the
 * Tile, and returns as pixel coords*/
void calculate_tile_minmax(
	TIL_Tile* tile,
	WORLDPIXELCOORDS& min,
	WORLDPIXELCOORDS& max);

/** Returns the 'world' pixel X, Y coordinates from a tile, which Houdini
 * provides in coordinates relative to the size of the tile. */
WORLDPIXELCOORDS calculate_world_pixel(
	TIL_TileList* tiles,
	TIL_Tile* tile,
	int pixel_index);

/**Object that stores and manipulates the overall 'offset'
 * between the transformation space, and the desired viewing
 * space of a fractal.
 * The default size of the transformation space that shall be
 * compared against the object's image_x and image_y members.
 * These should be set, either from the constructor or from
 * set_image_size. Then, to 'transform' the fractal, call
 * set_xform with the desired transformation. With these two
 * values, FratalSpace will successfully return coordinates in
 * 'fractal space', which can be passed to fractals.
 */
class FractalSpace
{
	int image_x{ 0 };
	int image_y{ 0 };
	UT_Matrix3D post_matrix; /**>The matrix that defines the fractal space.*/
	/**> Container for the literal data used to transform the post_matrix.*/
	XformStashData xdata;

	/**Private method which implements the get_fractal_coords
	 * methods to reduce code duplication. */
	COMPLEX _get_fractal_coords(COMPLEX pixel_coords);

public:

	FractalSpace();
	FractalSpace(int x, int y);

	void set_xform(MultiXformStashData& xdata);
	void set_xform(XformStashData& xdata);
	void set_xform(UT_Matrix3D& xform);

	/** Return the fractal coordinates, which use the size of the image
	 * as a relative size. The scale is 0-1 in the x axis of the image.
	 */
	COMPLEX get_fractal_coords(WORLDPIXELCOORDS pixel_coords);

	/**Returns fractal coordinates, but with input coordinates that
	 * were determined from double precision values, and not inteter
	 * pixels. Primarily used by the Buddhabrot where 'sample' values
	 * may not directly match a pixel. See COP2_Buddhabrot for more
	 * details.*/
	COMPLEX get_fractal_coords(COMPLEX pixel_coords);

	/**get_pixel_coords is only configured to work with the Buddhabrot,
	 * and may not work with non-default transformation orders or
	 * xforms with non-zero pivot points.*/
	WORLDPIXELCOORDS get_pixel_coords(COMPLEX fractal_coords);

	/**Returns in fractal coords at the bottom-left most pixel. */
	COMPLEX get_minimum();
	/**Returns in fractal coords at the top-right most pixel. */
	COMPLEX get_maximum();

	/** A fractal space must be populated with a default image size to be used
	 * by the context correctly. The image size is used as a rest size for the
	 * fractal, which is normalized by
	 * The size of the image. This is done so that changes in resolution
	 * don't require Changes in transformation to see the same Fractal.
	 */
	void set_image_size(int x, int y);

	/**Getter for the image size */
	WORLDPIXELCOORDS get_image_size();

	virtual ~FractalSpace();

};

} // End of CC Namespace