#include <cassert>

#include "bwPoint.h"
#include "bwPolygon.h"

#include "bwPainter.h"

using namespace bWidgets;


void (*bwPainter::drawPolygonCb)(const class bwPainter&, const class bwPolygon&) = 0;
void (*bwPainter::drawTextCb)(const class bwPainter &, const std::string&, const bwRectanglePixel&, void*) = 0;
void* bwPainter::text_draw_arg = NULL;

bwPainter::bwPainter() :
    active_drawtype(DRAW_TYPE_FILLED),
    active_gradient(nullptr)
{
	
}

void bwPainter::drawPolygon(const bwPolygon& poly) const
{
	if (poly.isDrawable()) {
		drawPolygonCb(*this, poly);
	}
}

void bwPainter::drawText(const std::string& text, const bwRectanglePixel& rectangle, void* text_draw_arg) const
{
	if (text.size() > 0) {
		drawTextCb(*this, text, rectangle, text_draw_arg);
	}
}

void bwPainter::setActiveColor(const bwColor& color)
{
	active_color = color;
	active_gradient = nullptr;
}

const bwColor& bwPainter::getActiveColor() const
{
	return active_color;
}

const bwColor& bwPainter::getVertexColor(const size_t vertex_index) const
{
	return vert_colors[vertex_index];
}

void bwPainter::enableGradient(
        const bwColor& base_color,
        const float shade_begin, const float shade_end,
        const bwGradient::Direction direction)
{
	if (!active_gradient) {
		active_gradient = std::unique_ptr<bwGradient>(new bwGradient());
	}

	active_gradient->begin = base_color;
	active_gradient->begin.shade(shade_begin);
	active_gradient->end = base_color;
	active_gradient->end.shade(shade_end);
	active_gradient->direction = direction;
}

bool bwPainter::isGradientEnabled() const
{
	return active_gradient != nullptr;
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
        bwPolygon& polygon, const bwRectanglePixel& rect,
        unsigned int corners, const float radius, const bool is_outline)
{
	bwRectanglePixel rect_inner = rect;
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

void bwPainter::drawRoundbox(
        const bwRectanglePixel& rect,
        unsigned int corners, const float radius)
{
	bwPolygon polygon;

	PolygonRoundboxAddVerts(polygon, rect, corners, radius, active_drawtype == DRAW_TYPE_OUTLINE);
	if (isGradientEnabled()) {
		fillVertexColorsWithGradient(polygon, rect);
		assert(vert_colors.size() == polygon.getVertices().size());
	}
	drawPolygon(polygon);
}

/**
 * \param rect: The bounding box of the polygon based on which the gradient is drawn. It could be calculated just
 *              now since there's access to \a polygon, but it's usually available when calling this function anyway.
 */
void bwPainter::fillVertexColorsWithGradient(
        const bwPolygon& polygon, const bwRectanglePixel& bounding_box)
{
	assert(isGradientEnabled());

	vert_colors.reserve(polygon.getVertices().size());
	for (const bwPoint& vertex : polygon.getVertices()) {
		vert_colors.push_back(active_gradient->calcPointColor(vertex, bounding_box));
	}
}
