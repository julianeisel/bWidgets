#pragma once

#include <memory>
#include <vector>

#include "bwGradient.h"

namespace bWidgets {

enum RoundboxCorner {
	NONE  = 0,
	BOTTOM_LEFT  = (1 << 0),
	BOTTOM_RIGHT = (1 << 1),
	TOP_LEFT     = (1 << 2),
	TOP_RIGHT    = (1 << 3),
	/* Convenience */
	ALL = (BOTTOM_LEFT | BOTTOM_RIGHT | TOP_LEFT | TOP_RIGHT),
};

enum TextAlignment {
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_CENTER,
	TEXT_ALIGN_RIGHT,
};

class bwPainter
{
public:
	enum DrawType {
		DRAW_TYPE_FILLED,
		DRAW_TYPE_OUTLINE,
	} active_drawtype;

	static void (*drawPolygonCb)(
	        const class bwPainter& painter,
	        const class bwPolygon& poly);
	static void (*drawTextCb)(
	        const class bwPainter& painter,
	        const std::string& text,
	        const bwRectanglePixel& rectangle,
	        const TextAlignment alignment);

	bwPainter();

	void drawPolygon(
	        const class bwPolygon& poly);
	void drawText(
	        const std::string& text,
	        const bwRectanglePixel& rectangle,
	        const TextAlignment align) const;

	void setActiveColor(const bwColor& color);
	const bwColor& getActiveColor() const;
	const bwColor& getVertexColor(const size_t vertex_index) const;

	void setContentMask(const bwRectanglePixel& value);
	const bwRectanglePixel& getContentMask() const;

	void enableGradient(
	        const bwColor& base_color,
	        const float shade_begin, const float shade_end,
	        const bwGradient::Direction direction);
	bool isGradientEnabled() const;

	// Primitives
	void drawRoundbox(
	        const bwRectanglePixel& rect,
	        unsigned int corners, const float radius);
	void drawRectangle(
	        const bwRectanglePixel& rect);
	void drawRoundboxWidgetBase(
	        const class bwWidget& widget,
	        const class bwStyle& style,
	        const bwRectanglePixel rectangle);
	void drawCheckMark(
	        const bwRectanglePixel& rect);

private:
	bwColor active_color;
	std::vector<bwColor> vert_colors;
	std::unique_ptr<bwGradient> active_gradient;
	bwRectanglePixel content_mask;

	void fillVertexColorsWithGradient(
	        const bwPolygon& polygon, const bwRectanglePixel& bounding_box);
};

} // namespace bWidgets
