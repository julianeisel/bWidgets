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
	const bwWidgetStyle& widget_style = style.widget_styles[type];
	bwPainter painter;

	style.setWidgetStyle(*this);

	painter.drawRoundboxWidgetBase(*this, style);

	// Text
	painter.setActiveColor(widget_style.textColor(state));
	painter.drawText(text, rectangle, widget_style.text_alignment);
}

void bwAbstractButton::mousePressEvent(const bwWidget::MouseButton button)
{
	if (button == MOUSE_BUTTON_LEFT) {
		state = STATE_SUNKEN;
	}
}

void bwAbstractButton::mouseReleaseEvent(const bwWidget::MouseButton button)
{
	if ((button == MOUSE_BUTTON_LEFT) && (state == STATE_SUNKEN)) {
		state = STATE_NORMAL;
	}
}

void bwAbstractButton::mouseEnter()
{
	state = STATE_HIGHLIGHTED;
}

void bwAbstractButton::mouseLeave()
{
	state = STATE_NORMAL;
}
