#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Color.h"
#include "Gradient.h"
#include "Rectangle.h"

namespace bWidgets {

class Painter
{
public:
	static void (*drawPolygonCb)(const class Painter& painter, const class Polygon& poly);
	static void (*drawTextCb)(
	        const class Painter& painter, const std::string& text,
	        const Rectangle<unsigned int>& rectangle, void* text_draw_arg);
	static void* text_draw_arg;

	Painter();

	void drawPolygon(const class Polygon& poly) const;
	void drawText(const std::string& text, const Rectangle<unsigned int>& rectangle, void* text_draw_arg) const;

	void setActiveColor(const Color& color);
	const Color& getActiveColor() const;
	const std::vector<Color> getVertexColors() const;

	void enableGradient(
	        const Color& base_color,
	        const float shade_begin, const float shade_end,
	        const Gradient::Direction direction);
	bool isGradientEnabled() const;

	enum DrawType {
		DRAW_TYPE_FILLED,
		DRAW_TYPE_OUTLINE,
	} active_drawtype;

	// Primitives
	void drawRoundbox(
	        const Rectangle<unsigned int>& rect,
	        unsigned int corners, const float radius);

private:
	Color active_color;
	std::vector<Color> vert_colors;
	std::unique_ptr<Gradient> active_gradient;

	void fillVertexColorsWithGradient(
	        const Polygon& polygon, const Rectangle<unsigned int>& bounding_box);
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
