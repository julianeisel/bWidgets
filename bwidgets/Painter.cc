#include "Painter.h"


using namespace bWidgets;

void (*Painter::drawPolygon)(const class Painter&, const class Polygon&) = 0;
void (*Painter::drawText)(const class Painter &, const std::string&, const class Widget&, void*) = 0;
void* Painter::text_draw_arg = NULL;

Painter::Painter() :
    active_drawtype(DRAW_TYPE_FILLED)
{
	
}

void Painter::setActiveColor(const Color& color)
{
	active_color = color;
}

const Color& Painter::getActiveColor() const
{
	return active_color;
}
