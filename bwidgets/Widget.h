#pragma once

namespace bWidgets {

class Widget
{
public:
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

	int pos_x, pos_y;
	int width, height;
};

} // namespace bWidgets
