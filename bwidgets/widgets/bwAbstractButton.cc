#include "bwStyle.h"

#include "bwAbstractButton.h"

using namespace bWidgets;


bwAbstractButton::bwAbstractButton(
        const std::string& text, const WidgetType type,
        const unsigned int width_hint, const unsigned int height_hint) :
    bwWidget(type, width_hint, height_hint),
    rounded_corners(RoundboxCorner::ALL),
    text(text)
{
	
}

void bwAbstractButton::draw(bwStyle& style) const
{
	const bwWidgetStyle& widget_style = style.widget_styles[type];
	bwPainter painter;

	style.setWidgetStyle(*this);

	painter.drawRoundboxWidgetBase(*this, style, rectangle);

	// Text
	painter.setActiveColor(widget_style.textColor(state));
	painter.drawText(text, rectangle, widget_style.text_alignment);
}

void bwAbstractButton::mousePressEvent(
        const bwWidget::MouseButton button,
        const bwPoint& /*location*/)
{
	if (button == MOUSE_BUTTON_LEFT) {
		state = STATE_SUNKEN;
	}
}

void bwAbstractButton::mouseReleaseEvent(
        const bwWidget::MouseButton button,
        const bwPoint& /*location*/)
{
	if ((button == MOUSE_BUTTON_LEFT) && (state == STATE_SUNKEN)) {
		state = STATE_NORMAL;
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
