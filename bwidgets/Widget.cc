#include "Widget.h"

using namespace bWidgets;

Widget::Widget(const Rectangle<unsigned int>& rectangle) :
    rectangle(rectangle)
{
	
}

bool Widget::isCoordinateInside(int x, int y)
{
	return rectangle.isCoordinateInside(x, y);
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
