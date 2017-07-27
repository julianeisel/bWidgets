#include <iostream>
#include <string>

#include "Painter.h"
#include "Polygon.h"
#include "Point.h"

#include "PushButton.h"

using namespace bWidgets;


PushButton::PushButton(
        const std::string& _text, int position_x, int position_y, int _width, int _height) :
    text(_text)
{
	pos_x = position_x;
	pos_y = position_y;
	width = _width;
	height = _height;
}

void PushButton::draw()
{
	std::vector<Point> rect = {
		Point(pos_x, pos_y),
		Point(pos_x + width, pos_y),
		Point(pos_x + width, pos_y + height),
		Point(pos_x, pos_y + height)
	};
	Polygon poly(rect);

	Painter::drawPolygon(poly, is_clicked);
	Painter::drawText(text, *this, Painter::text_draw_arg);
}

void PushButton::onClick()
{
	std::cout << text << std::endl;
	is_clicked = !is_clicked;
}
