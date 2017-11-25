#include "bwPainter.h"
#include "bwStyle.h"

#include "bwScrollBar.h"

using namespace bWidgets;


bwScrollBar::bwScrollBar(
        unsigned int width_hint, unsigned int height_hint) :
    bwAbstractButton("", WIDGET_TYPE_SCROLL_BAR, width_hint, height_hint)
{
	
}

void bwScrollBar::draw(bwStyle& style) const
{
	bwWidgetStyle& widget_style = style.widget_styles[type];
	bwRectanglePixel rect_inner{rectangle};
	/* TODO maybe a wrapper could ensure old style is unchanged after drawing (by resetting it) */
	const char old_shade_top = widget_style.shade_top;
	const char old_shade_bottom = widget_style.shade_bottom;
	bwPainter painter;

	rect_inner.ymax += (ratio * scroll_offset);
	rect_inner.ymin = rect_inner.ymax - (ratio * rectangle.height());

	style.setWidgetStyle(*this);

	std::swap(widget_style.shade_top, widget_style.shade_bottom);
	painter.drawRoundboxWidgetBase(
	            widget_style, bwWidget::STATE_NORMAL, style, rectangle,
	            bwGradient::DIRECTION_LEFT_RIGHT);

	std::swap(widget_style.shade_top, widget_style.shade_bottom);
	if (widget_style.shade_bottom == widget_style.shade_top) {
		// no shading, skip
	}
	else if (widget_style.shade_top > widget_style.shade_bottom) {
		widget_style.shade_top += 20;
	}
	else {
		widget_style.shade_bottom += 20;
	}

	painter.drawRoundboxWidgetBase(
	            widget_style, state, style, rect_inner,
	            bwGradient::DIRECTION_LEFT_RIGHT,
	            bwWidgetStyle::WIDGET_STYLE_COLOR_DECORATION);

	widget_style.shade_top = old_shade_top;
	widget_style.shade_bottom = old_shade_bottom;
}

void bwScrollBar::mousePressEvent(
        const bwWidget::MouseButton button,
        const bwPoint& location)
{
	bwAbstractButton::mousePressEvent(button, location);
	mouse_press_scroll_offset = scroll_offset;
}

void bwScrollBar::mouseReleaseEvent(
        const bwWidget::MouseButton button,
        const bwPoint& location)
{
	bwAbstractButton::mouseReleaseEvent(button, location);
}

void bwScrollBar::mouseDragEvent(
        const bwWidget::MouseButton button,
        const bwDistance drag_distance)
{
	if (button == MOUSE_BUTTON_LEFT) {
		setScrollOffset(mouse_press_scroll_offset + drag_distance.y);
	}
}

void bwScrollBar::setScrollOffset(int value)
{
	scroll_offset = value;
	apply();
}
