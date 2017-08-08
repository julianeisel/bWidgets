#include "Widget.h"

using namespace bWidgets;

bool Widget::isCoordinateInside(int x, int y)
{
	if ((x > pos_x) && (x < (pos_x + width))) {
		if ((y > pos_y) && (y < (pos_y + height))) {
			return true;
		}
	}

	return false;
}

void Widget::onClick()
{
	if (state == STATE_SUNKEN) {
		state = STATE_NORMAL;
	}
	else {
		state = STATE_SUNKEN;
	}
}

void Widget::mouseEnter()
{
	if (state == STATE_NORMAL) {
		state = STATE_HIGHLIGHTED;
	}
}

void Widget::mouseLeave()
{
	if (state == STATE_HIGHLIGHTED) {
		state = STATE_NORMAL;
	}
}
