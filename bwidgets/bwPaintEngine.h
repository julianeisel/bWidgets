#pragma once

#include <string>

#include "bwRectangle.h"


namespace bWidgets {

enum TextAlignment: unsigned int;

/**
 * \class bwPaintEngine
 * \brief Main interface to implement any drawing of geometry or text.
 *
 * bwPaintEngine mainly gives us two benefits:
 * * Abstracting away any drawing to the application, out of bWidgets.
 * * Support multiple drawing backends (e.g. OpenGL and Vulcan).
 *
 * Having support for multiple drawing backends is not only useful for
 * supporting multiple graphics libraries (as in OpenGL, DirectX, Vulcan,
 * etc.), but also to draw on different mediums. E.g. for drawing into textures
 * or exporting as HTML & CSS code.
 *
 * Creating a paint-engine is done by subclassing this class and
 * implementing the abstract functions.
 *
 * \note Even though the name may suggest this is the equivalent to Qt's
 *       QPaintEngine, this is more of a combination of both, QPaintEngine
 *       and QPaintDevice.
 */
class bwPaintEngine
{
public:
	virtual ~bwPaintEngine() = default;

	/**
	 * Setup function for application to prepare viewport (prepare matrices,
	 * clear background, etc.) for drawing.
	 *
	 * \note bWidgets doesn't call this itself, this is just a utility for
	 *       the application to use.
	 *
	 * \param rect The rectangle defining the viewport location and size.
	 * \param clear_color Color to be used for buffer clearing.
	 */
	virtual void setupViewport(
	        const bwRectanglePixel& rect,
	        const class bwColor& clear_color) = 0;

	/**
	 * The main polygon draw function which is used to draw all geometry of
	 * widgets.
	 * \param painter Provides all aditional information needed for drawing
	 *                (color, draw-style, etc.).
	 */
	virtual void drawPolygon(
	        const class bwPainter& painter,
	        const class bwPolygon& polygon) = 0;
	/**
	 * The main text draw function which is used to draw all text of widgets.
	 * \param painter Provides all aditional information needed for drawing
	 *                (color, draw-style, etc.).
	 * \param rect The bounding-box to draw the text into.
	 */
	virtual void drawText(
	        const class bwPainter& painter,
	        const std::string& text,
	        const bwRectanglePixel& rect,
	        const TextAlignment alignment) = 0;
	/**
	 * Draw the icon referenced by \a icon_interface.
	 * \param rect The bounding-box to draw the icon into.
	 */
	virtual void drawIcon(
	        const class bwIconInterface& icon_interface,
	        const bwRectanglePixel& rect) = 0;
};

} // namespace bWidgets
