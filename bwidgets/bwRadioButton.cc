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

void bwRadioButton::onClick()
{
	state = STATE_SUNKEN;
	apply(*this);
}
