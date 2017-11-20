#pragma once

#include <string>

#include "bwColor.h"
#include "bwWidget.h"


namespace bWidgets {

enum TextAlignment: unsigned int;

class bwWidgetStyle
{
private:
	struct bwWidgetStateColors
	{
		bwColor normal;
		bwColor highlighted;
		bwColor sunken;
	};

public:
	enum WidgetStyleColorID {
		WIDGET_STYLE_COLOR_BACKGROUND,
		WIDGET_STYLE_COLOR_TEXT,
		WIDGET_STYLE_COLOR_OUTLINE,
		WIDGET_STYLE_COLOR_DECORATION,

		WIDGET_STYLE_COLOR_TOT
	};

	const bwColor& getColor(const WidgetStyleColorID, const bwWidget::WidgetState) const;
	const bwColor& backgroundColor(const bwWidget::WidgetState state) const;
	const bwColor& textColor(const bwWidget::WidgetState state) const;
	const bwColor& outlineColor(const bwWidget::WidgetState state) const;
	const bwColor& decorationColor(const bwWidget::WidgetState state) const;
	float shadeTop(const bwWidget::WidgetState state) const;
	float shadeBottom(const bwWidget::WidgetState state) const;

	bwWidgetStateColors state_colors[WIDGET_STYLE_COLOR_TOT];
	char shade_top, shade_bottom;
	bool invert_shade_on_sunken;
	TextAlignment text_alignment;
	float roundbox_radius;
	unsigned int roundbox_corners;
};

class bwStyle
{
public:
	enum StyleTypeID {
		STYLE_BLENDER_CLASSIC,
		STYLE_BLENDER_FLAT,

		STYLE_BUILTIN_TOT,

//		STYLE_CUSTOM, // For the future
	} type_id;

	virtual ~bwStyle() {}

	virtual void setWidgetStyle(const class bwWidget& widget) = 0;

	bwWidgetStyle widget_styles[bwWidget::WIDGET_TYPE_TOT];

	float dpi_fac;

	struct StyleType
	{
		StyleTypeID type_id;
		std::string name;
	};

protected:
	bwStyle(StyleTypeID type_id, const bwWidgetStyle (&widget_styles)[bwWidget::WIDGET_TYPE_TOT]);
};

} // namespace bWidgets
