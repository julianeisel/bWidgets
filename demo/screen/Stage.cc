#include <iostream>
#include <list>

// drawing
extern "C" {
#include "../../extern/gawain/gawain/immediate.h"
}
#include "../gpu/ShaderProgram.h"

// bWidgets lib
#include "Color.h"
#include "Widget.h"
#include "Painter.h"
#include "Polygon.h"
#include "Point.h"

#include "Font.h"
#include "GPU.h"
#include "Window.h"

#include "Stage.h"


using namespace bWidgetDemo;

static PrimitiveType stage_polygon_drawtype_convert(const bWidgets::Painter::DrawType& drawtype)
{
	switch (drawtype) {
		case bWidgets::Painter::DrawType::DRAW_TYPE_FILLED:
			return PRIM_TRIANGLE_FAN;
		case bWidgets::Painter::DrawType::DRAW_TYPE_OUTLINE:
			return PRIM_LINE_LOOP;
	}
}

static void stage_polygon_draw(
        const bWidgets::Polygon& poly, const bWidgets::Color& color,
        const PrimitiveType type, const unsigned int attr_pos)
{
	glLineWidth(3.0f);
	immUniformColor4fv(color.getColor());

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
	unsigned int pos = VertexFormat_add_attrib(immVertexFormat(), "pos", COMP_F32, 2, KEEP_FLOAT);

	immBindProgram(shader_program.ProgramID(), shader_program.getInterface());
	stage_polygon_draw(poly, painter.getActiveColor(), stage_polygon_drawtype_convert(painter.active_drawtype), pos);
	immUnbindProgram();
}

/**
 * The main text draw callback which is used to draw all text of widgets.
 */
static void stage_text_draw_cb(const bWidgets::Painter &painter, const std::string& text,
                               const bWidgets::Widget& widget, void* text_draw_arg)
{
	Font* font = reinterpret_cast<Font*>(text_draw_arg);
	const float height = font->getSize();
//	const float width = font->calculateStringWidth(text);
//	const float draw_pos_x = widget.pos_x + ((widget.width - width) / 2.0f);
	const float draw_pos_x = widget.pos_x + 10.0f;
	const float draw_pos_y = widget.pos_y + ((widget.height - height) / 2.0f) + 1.0f;

	font->setActiveColor(painter.getActiveColor());
	font->render(text, draw_pos_x, draw_pos_y);
}

Stage::Stage(const unsigned int width, const unsigned int height) :
    width(width), height(height)
{
	// Initialize drawing callbacks for the stage
	bWidgets::Painter::drawPolygon = stage_polygon_draw_cb;
	bWidgets::Painter::drawText = stage_text_draw_cb;

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

bWidgets::Widget& Stage::getWidgetAt(const unsigned int index)
{
	unsigned int i = 0;
	for (bWidgets::Widget* widget : widgets) {
		if (i++ == index) {
			return *widget;
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
