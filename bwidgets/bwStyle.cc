#include <cassert>

#include "bwPainter.h"

#include "bwStyle.h"

using namespace bWidgets;


bwStyle::bwStyle(StyleTypeID type_id, bwWidgetStyleArray& widget_styles) :
    type_id(type_id), widget_styles(widget_styles), dpi_fac(1.0f)
{
	
}

bwWidgetStyle::bwWidgetStyle() :
	text_alignment(TEXT_ALIGN_LEFT), roundbox_corners(NONE)

{

}

const bwColor& bwWidgetStyle::getColor(
        const WidgetStyleColorID color_id,
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
