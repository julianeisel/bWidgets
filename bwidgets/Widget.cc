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
