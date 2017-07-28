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
	Color col_regular(0.6f, 0.6f, 0.6f);
	Color col_clicked(0.353f, 0.353f, 0.353f);
	Color col_text_regular(0.0f, 0.0f, 0.0f);
	Color col_text_clicked(1.0f, 1.0f, 1.0f);
	Color outline(0.0f, 0.0f, 0.0f);
	Painter painter;

	std::vector<Point> rect = {
		Point(pos_x, pos_y),
		Point(pos_x + width, pos_y),
		Point(pos_x + width, pos_y + height),
		Point(pos_x, pos_y + height)
	};
	Polygon poly(rect);

	// Inner
	painter.setActiveColor(is_clicked ? col_clicked : col_regular);
	painter.drawPolygon(painter, poly);
	// Outline
	painter.setActiveColor(outline);
	painter.active_drawtype = Painter::DrawType::DRAW_TYPE_OUTLINE;
	painter.drawPolygon(painter, poly);

	// Text
	painter.setActiveColor(is_clicked ? col_text_clicked : col_text_regular);
	painter.drawText(painter, text, *this, Painter::text_draw_arg);
}

void PushButton::onClick()
{
	std::cout << text << std::endl;
	is_clicked = !is_clicked;
}
