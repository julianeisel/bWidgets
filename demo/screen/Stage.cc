#include <iostream>
#include <list>

// drawing
extern "C" {
#include "../../extern/gawain/gawain/immediate.h"
}
#include "../gpu/ShaderProgram.h"

// bWidgets lib
#include "Color.h"
#include "Painter.h"
#include "Polygon.h"
#include "Point.h"
#include "Rectangle.h"
#include "Widget.h"

#include "Font.h"
#include "GPU.h"
#include "Window.h"

#include "Stage.h"


using namespace bWidgetDemo;

#define WIDGET_AA_JITTER 8

static const float jit[WIDGET_AA_JITTER][2] = {
	{ 0.468813, -0.481430}, {-0.155755, -0.352820},
	{ 0.219306, -0.238501}, {-0.393286, -0.110949},
	{-0.024699,  0.013908}, { 0.343805,  0.147431},
	{-0.272855,  0.269918}, { 0.095909,  0.388710}
};


static PrimitiveType stage_polygon_drawtype_convert(const bWidgets::Painter::DrawType& drawtype)
{
	switch (drawtype) {
		case bWidgets::Painter::DRAW_TYPE_FILLED:
			return PRIM_TRIANGLE_FAN;
		case bWidgets::Painter::DRAW_TYPE_OUTLINE:
			return PRIM_TRIANGLE_STRIP;
	}

	return PRIM_NONE;
}

static void stage_polygon_draw(
        const bWidgets::Polygon& poly, const bWidgets::Color& color,
        const PrimitiveType type, const unsigned int attr_pos)
{
	immUniformColor4fv(color);

	immBegin(type, poly.getVertices().size());
	for (const bWidgets::Point& vertex : poly.getVertices()) {
		immVertex2f(attr_pos, vertex.x, vertex.y);
	}
	immEnd();
}

/**
 * The main polygon draw callback which is used to draw all geometry of widgets.
 */
static void stage_polygon_draw_cb(const bWidgets::Painter& painter, const bWidgets::Polygon& poly)
{
	ShaderProgram shader_program(ShaderProgram::ID_UNIFORM_COLOR);
	bWidgets::Color color = painter.getActiveColor();
	PrimitiveType prim_type = stage_polygon_drawtype_convert(painter.active_drawtype);
	unsigned int pos = VertexFormat_add_attrib(immVertexFormat(), "pos", COMP_F32, 2, KEEP_FLOAT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	immBindProgram(shader_program.ProgramID(), shader_program.getInterface());

	if (painter.active_drawtype == bWidgets::Painter::DRAW_TYPE_OUTLINE) {
		bWidgets::Color drawcolor = color;

		drawcolor[3] /= WIDGET_AA_JITTER;

		for (int i = 0; i < WIDGET_AA_JITTER; i++) {
			gpuTranslate2f(jit[i]);
			stage_polygon_draw(poly, drawcolor, prim_type, pos);
			gpuTranslate2f(-jit[i][0], -jit[i][1]);
		}
	}
	else {
		stage_polygon_draw(poly, color, prim_type, pos);
	}

	immUnbindProgram();
	glDisable(GL_BLEND);
}

/**
 * The main text draw callback which is used to draw all text of widgets.
 */
static void stage_text_draw_cb(
        const bWidgets::Painter &painter, const std::string& text,
        const bWidgets::Rectangle<unsigned int>& rectangle, void* text_draw_arg)
{
	Font* font = reinterpret_cast<Font*>(text_draw_arg);
	const float font_height = font->getSize();
//	const float font_width = font->calculateStringWidth(text);
//	const float draw_pos_x = widget.rectangle.centerX() - (font_width / 2.0f);
	const float draw_pos_x = rectangle.xmin + 10.0f;
	const float draw_pos_y = rectangle.centerY() - (font_height / 2.0f) + 1.0f;

	font->setActiveColor(painter.getActiveColor());
	font->render(text, draw_pos_x, draw_pos_y);
}

Stage::Stage(const unsigned int width, const unsigned int height) :
    width(width), height(height)
{
	// Initialize drawing callbacks for the stage
	bWidgets::Painter::drawPolygonCb = stage_polygon_draw_cb;
	bWidgets::Painter::drawTextCb = stage_text_draw_cb;

	initFonts();
	bWidgets::Painter::text_draw_arg = font;
}

Stage::~Stage()
{
	for (bWidgets::Widget* widget : widgets) {
		delete widget;
	}

	delete font;
}

void Stage::initFonts()
{
	// Initialize freetype
	Font::initFontReading();

	// Initialize default font
	font = Font::loadFont("bfont.ttf", RESOURCES_PATH_STR);
	font->setSize(11.0f);
}

void Stage::draw(unsigned int width, unsigned int height)
{
	gpuOrtho(0.0f, width, 0.0f, height);

	for (bWidgets::Widget* widget : widgets) {
		widget->draw();
	}
}

void Stage::widgetAdd(bWidgets::Widget* widget)
{
	widgets.push_front(widget);
}

bWidgets::Widget* Stage::getWidgetAt(const unsigned int index)
{
	unsigned int i = 0;
	for (bWidgets::Widget* widget : widgets) {
		if (i++ == index) {
			return widget;
		}
	}

	return nullptr;
}

void Stage::handleMouseMovementEvent(const int mouse_xy[])
{
	for (bWidgets::Widget* widget : widgets) {
		const bool is_hovered = widget->isCoordinateInside(mouse_xy[0], mouse_xy[1]);

		if (widget->state == bWidgets::Widget::STATE_HIGHLIGHTED) {
			if (!is_hovered) {
				widget->mouseLeave();
			}
		}
		else if (is_hovered) {
			widget->mouseEnter();
		}
	}
}

void Stage::handleMouseButtonEvent(
        const Window& /*win*/, const int mouse_xy[2],
        int /*button*/, int /*action*/, int /*mods*/)
{
	for (bWidgets::Widget* widget : widgets) {
		if (widget->isCoordinateInside(mouse_xy[0], mouse_xy[1])) {
			widget->onClick();
		}
	}
}

void Stage::handleResizeEvent(const Window& win)
{
	width = win.getWidth();
	height = win.getHeight();
}

unsigned int Stage::getWidth() const
{
	return width;
}

unsigned int Stage::getHeight() const
{
	return height;
}
