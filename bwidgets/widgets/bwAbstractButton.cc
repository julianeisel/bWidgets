#include "bwStyle.h"

#include "bwAbstractButton.h"

using namespace bWidgets;


bwAbstractButton::bwAbstractButton(
        const std::string& text, const WidgetType type,
        const bwRectanglePixel& rectangle) :
    bwWidget(type, rectangle), rounded_corners(RoundboxCorner::ALL), text(text)
{
	
}

void bwAbstractButton::draw(bwStyle& style) const
{
	bwWidgetStyle& widget_style = style.widget_styles[type];
	bwRectanglePixel inner_rect = rectangle;
	bwPainter painter;

	// Inner - "inside" of outline, so scale down
	inner_rect.resize(-1);

	style.setWidgetStyle(*this);
	painter.setContentMask(inner_rect);

	painter.enableGradient(
	            widget_style.backgroundColor(state),
	            widget_style.shadeTop(state, true), widget_style.shadeBottom(state, true),
	            bwGradient::DIRECTION_TOP_BOTTOM);
	painter.drawRoundbox(inner_rect, widget_style.roundbox_corners, widget_style.roundbox_radius - 1.0f);
	// Outline
	painter.setActiveColor(widget_style.outlineColor(state));
	painter.active_drawtype = bwPainter::DrawType::DRAW_TYPE_OUTLINE;
	painter.drawRoundbox(rectangle, widget_style.roundbox_corners, widget_style.roundbox_radius);

	// Text
	painter.setActiveColor(widget_style.textColor(state));
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
