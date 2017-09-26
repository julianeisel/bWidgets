#include "bwRadioButton.h"

using namespace bWidgets;

bwRadioButton::bwRadioButton(
        const std::string& text,
        unsigned int width_hint, unsigned int height_hint) :
    bwAbstractButton(text, WIDGET_TYPE_RADIO_BUTTON, width_hint, height_hint)
{
	
}

void bwRadioButton::mousePressEvent(const bwWidget::MouseButton button)
{
	if (button == bwWidget::MOUSE_BUTTON_LEFT) {
		state = STATE_SUNKEN;
		apply(*this);
	}
}

void bwRadioButton::mouseReleaseEvent(const bwWidget::MouseButton /*button*/)
{
	
}

void bwRadioButton::mouseEnter()
{
	if (state != STATE_SUNKEN) {
		state = STATE_HIGHLIGHTED;
	}
}

void bwRadioButton::mouseLeave()
{
	if (state != STATE_SUNKEN) {
		state = STATE_NORMAL;
	}
}
