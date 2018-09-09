#include <algorithm>
#include <cassert>
#include <iostream>

#include "bwPaintEngine.h"
#include "bwPoint.h"
#include "bwPolygon.h"
#include "bwRange.h"
#include "bwStyle.h"
#include "bwWidgetBaseStyle.h"

#include "bwPainter.h"

using namespace bWidgets;


bwPointer<bwPaintEngine> bwPainter::paint_engine = nullptr;

bwPainter::bwPainter() :
    active_drawtype(DRAW_TYPE_FILLED),
    active_gradient(nullptr)
{
	
}

static bool painter_check_paint_engine(const bwPainter& painter)
{
	if (painter.paint_engine == nullptr) {
		std::cout << PRETTY_FUNCTION << "-- Error: No paint-engine set!" << std::endl;
		return false;
	}

	return true;
}

void bwPainter::drawPolygon(
        const bwPolygon& poly)
{
	if (!painter_check_paint_engine(*this)) {
		return;
	}

	if (poly.isDrawable()) {
		paint_engine->drawPolygon(*this, poly);
	}
	if (isGradientEnabled()) {
		vert_colors.clear();
	}
}

void bwPainter::drawText(
        const std::string& text,
        const bwRectanglePixel& rectangle,
        const TextAlignment alignment) const
{
	if (!painter_check_paint_engine(*this)) {
		return;
	}

	if (text.size() > 0) {
		paint_engine->drawText(*this, text, rectangle, alignment);
	}
}

void bwPainter::drawIcon(
        const bwIconInterface& icon_interface,
        const bwRectanglePixel& rect) const
{
	if (!painter_check_paint_engine(*this)) {
		return;
	}

	if (!rect.isEmpty() && icon_interface.isValid()) {
		paint_engine->drawIcon(icon_interface, rect);
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

void bwPainter::setContentMask(const bwRectanglePixel& value)
{
	content_mask = value;
}

const bwRectanglePixel& bwPainter::getContentMask() const
{
	return content_mask;
}

void bwPainter::enableGradient(const bwGradient& gradient)
{
	if (!active_gradient) {
		active_gradient = bwPointer_new<bwGradient>();
	}

	*active_gradient = gradient;
}

bool bwPainter::isGradientEnabled() const
{
	return active_gradient != nullptr;
}

// ------------------ Primitives ------------------

static const std::vector<bwPoint> check_mark_verts = {
	{-0.578579f, 0.253369f},
	{-0.392773f, 0.412794f},
	{-0.004241f, -0.328551f},
	{-0.003001f, 0.034320f},
	{1.055313f, 0.864744f},
	{0.866408f, 1.026895f}
};

void bwPainter::drawCheckMark(
        const bwRectanglePixel& rect)
{
	const bwPoint center{(float)rect.centerX(), (float)rect.centerY()};
	const float size = 0.5f * rect.height();
	bwPolygon polygon;

	for (const bwPoint& point : check_mark_verts) {
		polygon.addVertex(size * point + center);
	}

	if (isGradientEnabled()) {
		fillVertexColorsWithGradient(polygon, rect);
	}

	drawPolygon(polygon);
}

void bwPainter::drawTriangle(
        const bwRectanglePixel& rect,
        Direction direction)
{
	bwPolygon polygon{3};

	switch (direction) {
		case DIRECTION_UP:
			polygon.addVertex(rect.xmin, rect.ymin);
			polygon.addVertex(rect.xmin + (rect.width() * 0.5f), rect.ymax);
			polygon.addVertex(rect.xmax, rect.ymin);
			break;
		case DIRECTION_DOWN:
			polygon.addVertex(rect.xmin, rect.ymax);
			polygon.addVertex(rect.xmin + (rect.width() * 0.5f), rect.ymin);
			polygon.addVertex(rect.xmax, rect.ymax);
			break;
		case DIRECTION_LEFT:
			polygon.addVertex(rect.xmax, rect.ymax);
			polygon.addVertex(rect.xmin, rect.ymin + (rect.height() * 0.5f));
			polygon.addVertex(rect.xmax, rect.ymin);
			break;
		case DIRECTION_RIGHT:
			polygon.addVertex(rect.xmin, rect.ymax);
			polygon.addVertex(rect.xmax, rect.ymin + (rect.height() * 0.5f));
			polygon.addVertex(rect.xmin, rect.ymin);
			break;
	}

	if (isGradientEnabled()) {
		fillVertexColorsWithGradient(polygon, rect);
	}

	drawPolygon(polygon);
}

void bwPainter::drawLine(
        const bwPoint& from,
        const bwPoint& to)
{
	bwPolygon polygon{2};

	active_drawtype = bwPainter::DRAW_TYPE_LINE;
	polygon.addVertex(from.x, from.y);
	polygon.addVertex(to.x, to.y);
	drawPolygon(polygon);
}

namespace bWidgets {
class PolygonRoundboxCreator
{
public:
	PolygonRoundboxCreator(
	        const bwRectanglePixel& rect,
	        const unsigned int corners,
	        const float radius,
	        const bool is_outline);

	void addVerts(bwPolygon& polygon);

private:
	void startRoundbox(const bwPolygon& polygon);
	void endRoundbox(bwPolygon& polygon) const;
	void addVertsBottomLeft(bwPolygon& polygon) const;
	void addVertsBottomRight(bwPolygon& polygon) const;
	void addVertsTopRight(bwPolygon& polygon) const;
	void addVertsTopLeft(bwPolygon& polygon) const;

	static const int ROUNDCORNER_RESOLUTION = 9;
	static constexpr float cornervec[ROUNDCORNER_RESOLUTION][2] = {
		{0.0f, 0.0f},
		{0.195f, 0.02f},
		{0.383f, 0.067f},
		{0.55f, 0.169f},
		{0.707f, 0.293f},
		{0.831f, 0.45f},
		{0.924f, 0.617f},
		{0.98f, 0.805f},
		{1.0f, 1.0f}
	};

	bwRectanglePixel rect;
	bwRectanglePixel rect_inner;

	float vec_outer[ROUNDCORNER_RESOLUTION][2];
	float vec_inner[ROUNDCORNER_RESOLUTION][2];

	int start_vertex_count;
	unsigned int corners;
	float radius;
	float radius_inner;
	bool is_outline;
};
} // namespace bWidgets
constexpr float PolygonRoundboxCreator::cornervec[ROUNDCORNER_RESOLUTION][2];

void PolygonRoundboxCreator::addVertsBottomLeft(bwPolygon& polygon) const
{
	if (corners & BOTTOM_LEFT) {
		for (int i = 0; i < ROUNDCORNER_RESOLUTION; i++) {
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
}

void PolygonRoundboxCreator::addVertsBottomRight(bwPolygon& polygon) const
{
	if (corners & BOTTOM_RIGHT) {
		for (int i = 0; i < ROUNDCORNER_RESOLUTION; i++) {
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
}

void PolygonRoundboxCreator::addVertsTopRight(bwPolygon& polygon) const
{
	if (corners & TOP_RIGHT) {
		for (int i = 0; i < ROUNDCORNER_RESOLUTION; i++) {
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
}

void PolygonRoundboxCreator::addVertsTopLeft(bwPolygon& polygon) const
{
	if (corners & TOP_LEFT) {
		for (int i = 0; i < ROUNDCORNER_RESOLUTION; i++) {
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
}

PolygonRoundboxCreator::PolygonRoundboxCreator(
        const bwRectanglePixel& rect,
        const unsigned int corners,
        const float _radius,
        const bool is_outline) :
    rect(rect), rect_inner(rect),
    corners(corners),
    radius(_radius), radius_inner(_radius - 1.0f),
    is_outline(is_outline)
{
	for (int i = 0; i < ROUNDCORNER_RESOLUTION; i++) {
		vec_outer[i][0] = radius * cornervec[i][0];
		vec_outer[i][1] = radius * cornervec[i][1];

		vec_inner[i][0] = radius_inner * cornervec[i][0];
		vec_inner[i][1] = radius_inner * cornervec[i][1];
	}
	rect_inner.resize(-1);
}

void PolygonRoundboxCreator::startRoundbox(const bwPolygon& polygon)
{
	start_vertex_count = polygon.getVertices().size();
}

void PolygonRoundboxCreator::endRoundbox(bwPolygon& polygon) const
{
	unsigned int first_vert_index = std::max(0, start_vertex_count - 1);
	// Back to start
	polygon.addVertex(polygon[first_vert_index]);
	if (is_outline) {
		polygon.addVertex(polygon[first_vert_index + 1]);
	}
}

void PolygonRoundboxCreator::addVerts(bwPolygon& polygon)
{
	// Assumes all corners are rounded so may reserve more than needed. That's fine though.
	polygon.reserve((ROUNDCORNER_RESOLUTION * 4 + 1) * (is_outline ? 2 : 1));

	startRoundbox(polygon);

	addVertsBottomLeft(polygon);
	addVertsBottomRight(polygon);
	addVertsTopRight(polygon);
	addVertsTopLeft(polygon);

	endRoundbox(polygon);
}

static unsigned int getRoundboxMinsize(
        const bwRectanglePixel& rect,
        unsigned int corners)
{
	const int hnum = ((corners & (TOP_LEFT | TOP_RIGHT)) == (TOP_LEFT | TOP_RIGHT) ||
	                  (corners & (BOTTOM_RIGHT | BOTTOM_LEFT)) == (BOTTOM_RIGHT | BOTTOM_LEFT)) ?
	                     1 : 2;
	const int vnum = ((corners & (TOP_LEFT | BOTTOM_LEFT)) == (TOP_LEFT | BOTTOM_LEFT) ||
	                  (corners & (TOP_RIGHT | BOTTOM_RIGHT)) == (TOP_RIGHT | BOTTOM_RIGHT)) ?
	                     1 : 2;

	return std::min(rect.width() * hnum, rect.height() * vnum);
}

void bwPainter::drawRoundbox(
        const bwRectanglePixel& rect,
        unsigned int corners, const float radius)
{
	bwPolygon polygon;
	const unsigned int minsize = getRoundboxMinsize(rect, corners);
	float validated_radius = radius;

	bwRange<float>::clampValue(validated_radius, 0.0f, minsize * 0.5f);

	PolygonRoundboxCreator roundbox_creator{rect, corners, validated_radius, active_drawtype == DRAW_TYPE_OUTLINE};
	roundbox_creator.addVerts(polygon);

	if (isGradientEnabled()) {
		fillVertexColorsWithGradient(polygon, rect);
	}
	drawPolygon(polygon);
}

void bwPainter::drawRectangle(const bwRectanglePixel& rect)
{
	bwPolygon polygon;

	polygon.addVertex(rect.xmin, rect.ymin);
	polygon.addVertex(rect.xmax, rect.ymin);
	polygon.addVertex(rect.xmax, rect.ymax);
	polygon.addVertex(rect.xmin, rect.ymax);

	if (isGradientEnabled()) {
		fillVertexColorsWithGradient(polygon, rect);
	}

	drawPolygon(polygon);
}

/**
 * \param rect: The bounding box of the polygon based on which the gradient is drawn. It could be calculated just
 *              now since there's access to \a polygon, but it's usually available when calling this function anyway.
 */
void bwPainter::fillVertexColorsWithGradient(const bwPolygon& polygon, const bwRectanglePixel& bounding_box)
{
	const bwPointVec& vertices = polygon.getVertices();
	const bool is_single_color = active_gradient->begin == active_gradient->end;

	assert(isGradientEnabled());

	vert_colors.reserve(vertices.size());
	for (const bwPoint& vertex : vertices) {
		const bwColor& col = is_single_color ?
		                         active_gradient->begin :
		                         active_gradient->calcPointColor(vertex, bounding_box);

		vert_colors.push_back(col);
	}

	assert(vert_colors.size() == vertices.size());
}

void bwPainter::drawRoundboxWidgetBase(
        const bwWidgetBaseStyle& base_style,
        const bwStyle& style,
        const bwRectanglePixel& rectangle,
        const bwGradient& gradient,
        const float radius)
{
	bwRectanglePixel inner_rect = rectangle;
	const float radius_pixel = radius * style.dpi_fac;

	assert(bwRange<int>::isInside(base_style.shade_top,    -255, 255, true));
	assert(bwRange<int>::isInside(base_style.shade_bottom, -255, 255, true));

	// Inner - "inside" of outline, so scale down
	inner_rect.resize(-1);

	setContentMask(inner_rect); // Not sure if we should set this here.

	active_drawtype = bwPainter::DrawType::DRAW_TYPE_FILLED;
	enableGradient(gradient);
	drawRoundbox(inner_rect, base_style.roundbox_corners, radius_pixel - 1.0f);

	// Outline
	active_drawtype = bwPainter::DrawType::DRAW_TYPE_OUTLINE;
	setActiveColor(base_style.borderColor());
	drawRoundbox(rectangle, base_style.roundbox_corners, radius_pixel);
}
