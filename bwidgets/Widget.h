#pragma once

namespace bWidgets {

class Widget
{
public:
	virtual ~Widget() {}

	bool isCoordinateInside(int x, int y);

	virtual void draw() = 0;
	virtual void onClick() = 0;
	bool is_clicked = false;

	int pos_x, pos_y;
	int width, height;
};

} // namespace bWidgets
