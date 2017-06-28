#pragma once

namespace bWidgets {

class Widget
{
public:
	bool isCoordinateInside(int x, int y);

	virtual void draw() = 0;
	virtual void onClick() = 0;

protected:
	int pos_x, pos_y;
	int width, height;
};

} // namespace bWidgets
