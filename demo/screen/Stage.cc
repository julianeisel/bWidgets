#include <iostream>
#include <vector>

// drawing
extern "C" {
#include "../../extern/gawain/gawain/immediate.h"
}
#include "../gpu/ShaderProgram.h"

// bWidgets lib
#include "Widget.h"
#include "Painter.h"
#include "Polygon.h"
#include "Point.h"

#include "Font.h"
#include "GPU.h"

#include "Stage.h"


using namespace bWidgetDemo;

/**
 * The main polygon draw callback which is used to draw all geometry of widgets.
 */
static void stage_polygon_draw_cb(const bWidgets::Polygon& poly, bool is_clicked)
{
	ShaderProgram shader_program(ShaderProgram::ID_UNIFORM_COLOR);
	unsigned int pos = VertexFormat_add_attrib(immVertexFormat(), "pos", COMP_F32, 2, KEEP_FLOAT);
	const float col_clicked[3] = {0.5f, 0.5f, 0.5f};
	const float col[3] = {0.8f, 0.8f, 0.8f};

	immBindProgram(shader_program.ProgramID(), shader_program.getInterface());
	immUniformColor3fv(is_clicked ? col_clicked : col);

	immBegin(PRIM_TRIANGLE_FAN, 4);
	for (const bWidgets::Point& vertex : poly.getVertices()) {
		immVertex2f(pos, vertex.x, vertex.y);
	}
	immEnd();

	immUnbindProgram();
}

/**
 * The main text draw callback which is used to draw all text of widgets.
 */
static void stage_text_draw_cb(const std::string& text, const bWidgets::Widget& widget, void* text_draw_arg)
{
	Font* font = reinterpret_cast<Font*>(text_draw_arg);
	const float height = font->getSize();
	const float width = font->calculateStringWidth(text);
	const float draw_pos_x = widget.pos_x + ((widget.width - width) / 2.0f);
	const float draw_pos_y = widget.pos_y + ((widget.height - height) / 2.0f) + 1.0f;

	font->render(text, draw_pos_x, draw_pos_y);
}

Stage::Stage()
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

void Stage::handleMouseButtonEvent(Window* win, const int mouse_xy[2], int button, int action, int mods)
{
	for (bWidgets::Widget* widget : widgets) {
		if (widget->isCoordinateInside(mouse_xy[0], mouse_xy[1])) {
			widget->onClick();
		}
	}
}
