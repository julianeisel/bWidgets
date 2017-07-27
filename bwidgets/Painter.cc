#include "Painter.h"

using namespace bWidgets;

void (*Painter::drawPolygon)(const class Polygon&, bool) = 0;
void (*Painter::drawText)(const std::string&, const class Widget&, void*) = 0;
void* Painter::text_draw_arg = NULL;
