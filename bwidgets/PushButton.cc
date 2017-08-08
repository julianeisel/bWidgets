#include <iostream>
#include <string>

#include "Painter.h"
#include "Polygon.h"

#include "PushButton.h"

using namespace bWidgets;


PushButton::PushButton(
        const std::string& _text, unsigned int position_x, unsigned int position_y, unsigned int _width, unsigned int _height) :
    Widget(Rectangle<unsigned int>(position_x, position_x + _width, position_y, position_y + _height)),
    text(_text)
{
	
}

void PushButton::draw()
{
	Color col_regular(0.6f, 0.6f, 0.6f);
	Color col_clicked(0.353f, 0.353f, 0.353f);
	Color col_text_regular(0.0f, 0.0f, 0.0f);
	Color col_text_clicked(1.0f, 1.0f, 1.0f);
	Color outline(0.098f, 0.098f, 0.098f);
	Painter painter;
	const float roundbox_radius = 5.0f;
	const unsigned int roundbox_corners = RoundboxCorner::ALL;
	const bool draw_sunken = state == STATE_SUNKEN;

	Rectangle<unsigned int> inner_rect = rectangle;

	if (state == STATE_HIGHLIGHTED) {
		col_regular.shade(0.06f);
	}

	// Inner - "inside" of outline, so scale down
	inner_rect.resize(-1);
	painter.setActiveColor(draw_sunken ? col_clicked : col_regular);
	painter.drawRoundbox(inner_rect, roundbox_corners, roundbox_radius - 1.0f);
	// Outline
	painter.setActiveColor(outline);
	painter.active_drawtype = Painter::DrawType::DRAW_TYPE_OUTLINE;
	painter.drawRoundbox(rectangle, roundbox_corners, roundbox_radius);

	// Text
	painter.setActiveColor(draw_sunken ? col_text_clicked : col_text_regular);
	painter.drawText(text, rectangle, Painter::text_draw_arg);
}
