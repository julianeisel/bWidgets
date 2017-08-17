#include "bwWidget.h"

using namespace bWidgets;

bwWidget::bwWidget(const WidgetType type, const bwRectanglePixel& rectangle) :
    type(type), state(STATE_NORMAL),
    rectangle(rectangle), rounded_corners(RoundboxCorner::ALL)
{
	
}

bool bwWidget::isCoordinateInside(int x, int y)
{
	return rectangle.isCoordinateInside(x, y);
}

void bwWidget::onClick()
{
	if (state == STATE_SUNKEN) {
		state = STATE_NORMAL;
	}
	else {
		state = STATE_SUNKEN;
	}
}

void bwWidget::mouseEnter()
{
	if (state == STATE_NORMAL) {
		state = STATE_HIGHLIGHTED;
	}
}

void bwWidget::mouseLeave()
{
	if (state == STATE_HIGHLIGHTED) {
		state = STATE_NORMAL;
	}
}
