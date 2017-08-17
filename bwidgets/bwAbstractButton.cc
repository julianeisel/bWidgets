#include "bwAbstractButton.h"

using namespace bWidgets;

bwAbstractButton::bwAbstractButton(const WidgetType type, const bwRectanglePixel& rectangle) :
    bwWidget(type, rectangle), state(STATE_NORMAL), rounded_corners(RoundboxCorner::ALL)
{
	
}

void bwAbstractButton::onClick()
{
	if (state == STATE_SUNKEN) {
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
