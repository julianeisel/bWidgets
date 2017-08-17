#pragma once

#include <memory>
#include <string>
#include <vector>

#include "bwColor.h"
#include "bwGradient.h"
#include "bwRectangle.h"

namespace bWidgets {

class bwPainter
{
public:
	static void (*drawPolygonCb)(const class bwPainter& painter, const class bwPolygon& poly);
	static void (*drawTextCb)(
	        const class bwPainter& painter, const std::string& text,
	        const bwRectangle<unsigned int>& rectangle, void* text_draw_arg);
	static void* text_draw_arg;

	bwPainter();

	void drawPolygon(const class bwPolygon& poly) const;
	void drawText(const std::string& text, const bwRectangle<unsigned int>& rectangle, void* text_draw_arg) const;

	void setActiveColor(const bwColor& color);
	const bwColor& getActiveColor() const;
	const std::vector<bwColor> getVertexColors() const;

	void enableGradient(
	        const bwColor& base_color,
	        const float shade_begin, const float shade_end,
	        const bwGradient::Direction direction);
	bool isGradientEnabled() const;

	enum DrawType {
		DRAW_TYPE_FILLED,
		DRAW_TYPE_OUTLINE,
	} active_drawtype;

	// Primitives
	void drawRoundbox(
	        const bwRectangle<unsigned int>& rect,
	        unsigned int corners, const float radius);

private:
	bwColor active_color;
	std::vector<bwColor> vert_colors;
	std::unique_ptr<bwGradient> active_gradient;

	void fillVertexColorsWithGradient(
	        const bwPolygon& polygon, const bwRectangle<unsigned int>& bounding_box);
};

enum RoundboxCorner {
	NONE  = 0,
	BOTTOM_LEFT  = (1 << 0),
	BOTTOM_RIGHT = (1 << 1),
	TOP_LEFT     = (1 << 2),
	TOP_RIGHT    = (1 << 3),
	/* Convenience */
	ALL = (BOTTOM_LEFT | BOTTOM_RIGHT | TOP_LEFT | TOP_RIGHT),
};

} // namespace bWidgets
