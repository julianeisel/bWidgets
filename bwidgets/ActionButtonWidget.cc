#include <iostream>
#include <string>

#include "util/Polygon.h"
#include "util/Point.h"

#include "ActionButtonWidget.h"

using namespace bWidgets;


ActionButtonWidget::ActionButtonWidget(
        const std::string& _text, int position_x, int position_y, int _width, int _height) :
    text(_text)
{
	pos_x = position_x;
	pos_y = position_y;
	width = _width;
	height = _height;
}

void ActionButtonWidget::draw()
{
	std::vector<Point> rect = {
		Point(pos_x, pos_y),
		Point(pos_x + width, pos_y),
		Point(pos_x + width, pos_y + height),
		Point(pos_x, pos_y + height)
	};
	Polygon poly(rect);

	Polygon::draw(poly);
}

void ActionButtonWidget::onClick()
{
	std::cout << text << std::endl;
}
