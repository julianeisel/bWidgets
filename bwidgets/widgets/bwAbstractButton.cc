#include "bwStyle.h"

#include "bwAbstractButton.h"

using namespace bWidgets;


bwAbstractButton::bwAbstractButton(
        const std::string& text, const WidgetType type,
        const bwRectanglePixel& rectangle) :
    bwWidget(type, rectangle), state(STATE_NORMAL), rounded_corners(RoundboxCorner::ALL), text(text)
{
	
}

void bwAbstractButton::draw(bwStyle& style) const
{
	bwStyle::WidgetStyle& widget_style = style.widget_style;
	bwRectanglePixel inner_rect = rectangle;
	bwPainter painter;

	style.setWidgetStyle(*this);

	// Inner - "inside" of outline, so scale down
	inner_rect.resize(-1);
	painter.enableGradient(
	            widget_style.fill_color,
	            widget_style.shade_top / 255.0f, widget_style.shade_bottom / 255.0f,
	            bwGradient::DIRECTION_TOP_BOTTOM);
	painter.drawRoundbox(inner_rect, widget_style.roundbox_corners, widget_style.roundbox_radius - 1.0f);
	// Outline
	painter.setActiveColor(widget_style.outline_color);
	painter.active_drawtype = bwPainter::DrawType::DRAW_TYPE_OUTLINE;
	painter.drawRoundbox(rectangle, widget_style.roundbox_corners, widget_style.roundbox_radius);

	// Text
	painter.setActiveColor(widget_style.text_color);
	painter.drawText(text, rectangle, widget_style.text_alignment, bwPainter::text_draw_arg);
}

void bwAbstractButton::onClick(const MouseButton button)
{
	if (button == MOUSE_BUTTON_RIGHT) {
		// skip
	}
	else if (state == STATE_SUNKEN) {
		state = STATE_NORMAL;
	}
	else {
		state = STATE_SUNKEN;
	}
}

void bwAbstractButton::mouseEnter()
{
	if (state == STATE_NORMAL) {
		state = STATE_HIGHLIGHTED;
	}
}

void bwAbstractButton::mouseLeave()
{
	if (state == STATE_HIGHLIGHTED) {
		state = STATE_NORMAL;
	}
}
