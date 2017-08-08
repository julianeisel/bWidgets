#pragma once

#include "Rectangle.h"

namespace bWidgets {

class Widget
{
public:
	Widget(const Rectangle<unsigned int>& rectangle);
	virtual ~Widget() {}

	bool isCoordinateInside(int x, int y);

	virtual void draw() = 0;
	virtual void onClick();
	virtual void mouseEnter();
	virtual void mouseLeave();

	enum {
		STATE_NORMAL      = 0,
		STATE_HIGHLIGHTED = 1,
		STATE_SUNKEN      = 2,
	} state;

	Rectangle<unsigned int> rectangle;
};

} // namespace bWidgets
