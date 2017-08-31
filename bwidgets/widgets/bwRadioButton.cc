#include "bwPainter.h"
#include "bwStyle.h"

#include "bwRadioButton.h"

using namespace bWidgets;

bwRadioButton::bwRadioButton(
        const std::string& text,
        unsigned int position_x, unsigned int position_y,
        unsigned int width, unsigned int height) :
    bwAbstractButton(
        text, WIDGET_TYPE_RADIO_BUTTON,
        bwRectanglePixel(position_x, position_x + width, position_y, position_y + height))
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
