#include <cassert>

#include "bwStyle.h"

using namespace bWidgets;


bwStyle::bwStyle(StyleTypeID type_id, const bwWidgetStyle (&widget_styles)[bwWidget::WIDGET_TYPE_TOT]) :
    type_id(type_id), widget_styles(widget_styles)
{
	
}

const bwColor&bwWidgetStyle::backgroundColor(const bwWidget::WidgetState state) const
{
	return getColor(WIDGET_STYLE_COLOR_BACKGROUND, state);
}

const bwColor&bwWidgetStyle::textColor(const bwWidget::WidgetState state) const
{
	return getColor(WIDGET_STYLE_COLOR_TEXT, state);
}

const bwColor&bwWidgetStyle::outlineColor(const bwWidget::WidgetState state) const
{
	return getColor(WIDGET_STYLE_COLOR_OUTLINE, state);
}

const bwColor&bwWidgetStyle::decorationColor(const bwWidget::WidgetState state) const
{
	return getColor(WIDGET_STYLE_COLOR_DECORATION, state);
}

float bwWidgetStyle::shadeTop(const bwWidget::WidgetState state) const
{
	return ((invert_shade_on_sunken && (state == bwWidget::STATE_SUNKEN)) ? shade_bottom : shade_top) / 255.0f;
}

float bwWidgetStyle::shadeBottom(const bwWidget::WidgetState state) const
{
	return ((invert_shade_on_sunken && (state == bwWidget::STATE_SUNKEN)) ? shade_top : shade_bottom) / 255.0f;
}

const bwColor& bwWidgetStyle::getColor(
        const bwWidgetStyle::WidgetStyleColorID color_id,
        const bwWidget::WidgetState state) const
{
	const bwWidgetStateColors& colors = state_colors[color_id];

	if (state == bwWidget::STATE_NORMAL) {
		return colors.normal;
	}
	else if (state == bwWidget::STATE_HIGHLIGHTED) {
		return colors.highlighted;
	}

	assert(state == bwWidget::STATE_SUNKEN);
	return colors.sunken;
}
