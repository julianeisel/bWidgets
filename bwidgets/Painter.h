#pragma once

#include <string>

#include "Color.h"

namespace bWidgets {

class Painter {
public:
	static void (*drawPolygon)(const class Painter& painter, const class Polygon& poly);
	static void (*drawText)(const class Painter &painter, const std::string& text,
	                        const class Widget& widget, void* text_draw_arg);
	static void* text_draw_arg;

	Painter();

	void setActiveColor(const Color& color);
	const Color& getActiveColor() const;

	enum DrawType {
		DRAW_TYPE_FILLED,
		DRAW_TYPE_OUTLINE,
	} active_drawtype;

private:
	Color active_color;
};

} // namespace bWidgets
