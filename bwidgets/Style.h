#pragma once

#include "Color.h"

namespace bWidgets {

struct Style
{
	virtual void setWidgetStyle(const class Widget& widget) = 0;

	struct WidgetStyle {
		Color fill_color;
		Color text_color;
		Color outline_color;

		float roundbox_radius;
		unsigned int roundbox_corners;

		signed char shade_top, shade_bottom;
	} widget_style;
};

} // namespace bWidgets
