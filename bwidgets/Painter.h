#pragma once

#include <string>

namespace bWidgets {

class Painter {
public:
	static void (*drawPolygon)(const class Polygon& poly, bool is_clicked);
	static void (*drawText)(const std::string& text, const class Widget& widget, void* text_draw_arg);
	static void* text_draw_arg;
};

} // namespace bWidgets
