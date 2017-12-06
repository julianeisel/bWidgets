#pragma once

#include <array>
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

	bwWidgetStyle();

	const bwColor& getColor(const WidgetStyleColorID, const bwWidget::WidgetState) const;
	const bwColor& backgroundColor(const bwWidget::WidgetState state) const;
	const bwColor& textColor(const bwWidget::WidgetState state) const;
	const bwColor& outlineColor(const bwWidget::WidgetState state) const;
	const bwColor& decorationColor(const bwWidget::WidgetState state) const;
	float shadeTop(const bwWidget::WidgetState state) const;
	float shadeBottom(const bwWidget::WidgetState state) const;

	bwWidgetStateColors state_colors[WIDGET_STYLE_COLOR_TOT];
	char shade_top{0}, shade_bottom{0};
	bool invert_shade_on_sunken{false};
	TextAlignment text_alignment; // Default value set in constructor.
	float roundbox_radius{0.0f};  // TODO Utility function for calculating final radius (roundbox_radius * dpi_fac).
	unsigned int roundbox_corners; // Default value set in constructor.
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

	using bwWidgetStyleArray = std::array<bwWidgetStyle, bwWidget::WIDGET_TYPE_TOT>;
	bwWidgetStyleArray& widget_styles; // TODO Utility function for getting widget_style for widget.

	float dpi_fac;

	struct StyleType
	{
		StyleTypeID type_id;
		std::string name;
	};

protected:
	bwStyle(StyleTypeID type_id, bwWidgetStyleArray& widget_styles);
};

} // namespace bWidgets
