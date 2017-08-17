#pragma once

#include <string>

#include "Color.h"

namespace bWidgets {

struct Style
{
	enum StyleTypeID {
		STYLE_BLENDER_CLASSIC,
		STYLE_BLENDER_FLAT,

		STYLE_BUILTIN_TOT,

//		STYLE_CUSTOM, // For the future
	} type_id;

	struct WidgetStyle {
		Color fill_color;
		Color text_color;
		Color outline_color;

		float roundbox_radius;
		unsigned int roundbox_corners;

		signed char shade_top, shade_bottom;
	} widget_style;

	struct StyleType
	{
		StyleTypeID type_id;
		std::string name;
	};

	Style(StyleTypeID type_id);
	virtual void setWidgetStyle(const class Widget& widget) = 0;
};

} // namespace bWidgets
