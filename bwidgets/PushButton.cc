#include <iostream>
#include <string>

#include "Painter.h"
#include "Polygon.h"
#include "Style.h"

#include "PushButton.h"

using namespace bWidgets;


PushButton::PushButton(
        const std::string& _text, unsigned int position_x, unsigned int position_y, unsigned int _width, unsigned int _height) :
    Widget(Rectangle<unsigned int>(position_x, position_x + _width, position_y, position_y + _height)),
    text(_text)
{
	
}

void PushButton::draw(Style& style)
{
	Style::WidgetStyle& widget_style = style.widget_style;
	Rectangle<unsigned int> inner_rect = rectangle;
	Painter painter;

	style.setWidgetStyle(*this);

	// Inner - "inside" of outline, so scale down
	inner_rect.resize(-1);
	painter.enableGradient(
	            widget_style.fill_color,
	            widget_style.shade_top / 255.0f, widget_style.shade_bottom / 255.0f,
	            Gradient::DIRECTION_TOP_BOTTOM);
	painter.drawRoundbox(inner_rect, widget_style.roundbox_corners, widget_style.roundbox_radius - 1.0f);
	// Outline
	painter.setActiveColor(widget_style.outline_color);
	painter.active_drawtype = Painter::DrawType::DRAW_TYPE_OUTLINE;
	painter.drawRoundbox(rectangle, widget_style.roundbox_corners, widget_style.roundbox_radius);

	// Text
	painter.setActiveColor(widget_style.text_color);
	painter.drawText(text, rectangle, Painter::text_draw_arg);
}
