#include "Point.h"
#include "Polygon.h"

#include "Painter.h"

using namespace bWidgets;


void (*Painter::drawPolygonCb)(const class Painter&, const class Polygon&) = 0;
void (*Painter::drawTextCb)(const class Painter &, const std::string&, const class Widget&, void*) = 0;
void* Painter::text_draw_arg = NULL;

Painter::Painter() :
    active_drawtype(DRAW_TYPE_FILLED)
{
	
}

void Painter::drawPolygon(const Polygon& poly) const
{
	if (poly.isDrawable()) {
		drawPolygonCb(*this, poly);
	}
}

void Painter::drawText(const std::string& text, const Widget& widget, void* text_draw_arg) const
{
	if (text.size() > 0) {
		drawTextCb(*this, text, widget, text_draw_arg);
	}
}

void Painter::setActiveColor(const Color& color)
{
	active_color = color;
}

const Color& Painter::getActiveColor() const
{
	return active_color;
}

// ------------------ Primitives ------------------

#define WIDGET_CURVE_RESOLU 9

static const float cornervec[WIDGET_CURVE_RESOLU][2] = {
	{0.0, 0.0},
	{0.195, 0.02},
	{0.383, 0.067},
	{0.55, 0.169},
	{0.707, 0.293},
	{0.831, 0.45},
	{0.924, 0.617},
	{0.98, 0.805},
	{1.0, 1.0}
};

static void PolygonRoundboxAddVerts(
        Polygon& polygon, const Rectangle<unsigned int>& rect,
        unsigned int corners, const float radius, const bool is_outline)
{
	Rectangle<unsigned int> rect_inner = rect;
	const float radius_inner = radius - 1.0f;
	float vec_outer[WIDGET_CURVE_RESOLU][2];
	float vec_inner[WIDGET_CURVE_RESOLU][2];
	int i;

	for (i = 0; i < WIDGET_CURVE_RESOLU; i++) {
		vec_outer[i][0] = radius * cornervec[i][0];
		vec_outer[i][1] = radius * cornervec[i][1];

		vec_inner[i][0] = radius_inner * cornervec[i][0];
		vec_inner[i][1] = radius_inner * cornervec[i][1];
	}
	rect_inner.resize(-1);

	// Assumes all corners are rounded so may reserve more than needed. That's fine though.
	polygon.reserve((WIDGET_CURVE_RESOLU * 4 + 1) * (is_outline ? 2 : 1));

	if (corners & BOTTOM_LEFT) {
		for (i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			polygon.addVertex(rect.xmin + vec_outer[i][1], rect.ymin + radius - vec_outer[i][0]);
			if (is_outline) {
				polygon.addVertex(rect_inner.xmin + vec_inner[i][1], rect_inner.ymin + radius_inner - vec_inner[i][0]);
			}
		}
	}
	else {
		polygon.addVertex(rect.xmin, rect.ymin);
		if (is_outline) {
			polygon.addVertex(rect_inner.xmin, rect_inner.ymin);
		}
	}

	if (corners & BOTTOM_RIGHT) {
		for (i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			polygon.addVertex(rect.xmax - radius + vec_outer[i][0], rect.ymin + vec_outer[i][1]);
			if (is_outline) {
				polygon.addVertex(rect_inner.xmax - radius_inner + vec_inner[i][0], rect_inner.ymin + vec_inner[i][1]);
			}
		}
	}
	else {
		polygon.addVertex(rect.xmax, rect.ymin);
		if (is_outline) {
			polygon.addVertex(rect_inner.xmax, rect_inner.ymin);
		}
	}

	if (corners & TOP_RIGHT) {
		for (i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			polygon.addVertex(rect.xmax - vec_outer[i][1], rect.ymax - radius + vec_outer[i][0]);
			if (is_outline) {
				polygon.addVertex(rect_inner.xmax - vec_inner[i][1], rect_inner.ymax - radius_inner + vec_inner[i][0]);
			}
		}
	}
	else {
		polygon.addVertex(rect.xmax, rect.ymax);
		if (is_outline) {
			polygon.addVertex(rect_inner.xmax, rect_inner.ymax);
		}
	}

	if (corners & TOP_LEFT) {
		for (i = 0; i < WIDGET_CURVE_RESOLU; i++) {
			polygon.addVertex(rect.xmin + radius - vec_outer[i][0], rect.ymax - vec_outer[i][1]);
			if (is_outline) {
				polygon.addVertex(rect_inner.xmin + radius_inner - vec_inner[i][0], rect_inner.ymax - vec_inner[i][1]);
			}
		}
	}
	else {
		polygon.addVertex(rect.xmin, rect.ymax);
		if (is_outline) {
			polygon.addVertex(rect_inner.xmin, rect_inner.ymax);
		}
	}

	// Back to start
	if (corners & BOTTOM_LEFT) {
		polygon.addVertex(rect.xmin, rect.ymin + radius);
		if (is_outline) {
			polygon.addVertex(rect_inner.xmin, rect_inner.ymin + radius_inner);
		}
	}
	else {
		polygon.addVertex(rect.xmin, rect.ymin);
		if (is_outline) {
			polygon.addVertex(rect_inner.xmin, rect_inner.ymin);
		}
	}
}

void Painter::drawRoundbox(
        const Rectangle<unsigned int>& rect,
        unsigned int corners, const float radius)
{
	Polygon polygon;

	PolygonRoundboxAddVerts(polygon, rect, corners, radius, active_drawtype == DRAW_TYPE_OUTLINE);
	drawPolygon(polygon);
}
