#include <iostream>
#include <list>
#include <memory>

// drawing
extern "C" {
#include "../../extern/gawain/gawain/immediate.h"
}
#include "../gpu/ShaderProgram.h"

// bWidgets lib
#include "bwAbstractButton.h"
#include "bwColor.h"
#include "bwPainter.h"
#include "bwPolygon.h"
#include "bwPoint.h"
#include "bwRectangle.h"
#include "bwStyle.h"
#include "bwStyleManager.h"
#include "bwTextBox.h"
#include "bwWidget.h"

#include "Font.h"
#include "GPU.h"
#include "Window.h"

#include "Stage.h"


using namespace bWidgetsDemo;
using namespace bWidgets; // Less verbose

#define WIDGET_AA_JITTER 8

static const float jit[WIDGET_AA_JITTER][2] = {
	{ 0.468813, -0.481430}, {-0.155755, -0.352820},
	{ 0.219306, -0.238501}, {-0.393286, -0.110949},
	{-0.024699,  0.013908}, { 0.343805,  0.147431},
	{-0.272855,  0.269918}, { 0.095909,  0.388710}
};


static PrimitiveType stage_polygon_drawtype_convert(const bwPainter::DrawType& drawtype)
{
	switch (drawtype) {
		case bwPainter::DRAW_TYPE_FILLED:
			return PRIM_TRIANGLE_FAN;
		case bwPainter::DRAW_TYPE_OUTLINE:
			return PRIM_TRIANGLE_STRIP;
	}

	return PRIM_NONE;
}

static void stage_polygon_draw_uniform_color(
        const bwPolygon& poly, const bwColor& color,
        const PrimitiveType type, const unsigned int attr_pos)
{
	const bwPointVec& vertices = poly.getVertices();

	immUniformColor4fv(color);

	immBegin(type, vertices.size());
	for (const bwPoint& vertex : vertices) {
		immVertex2f(attr_pos, vertex.x, vertex.y);
	}
	immEnd();
}
static void stage_polygon_draw_shaded(
        const bwPainter& painter, const bwPolygon& poly, const PrimitiveType type,
        const unsigned int attr_pos, const unsigned int attr_color)
{
	const bwPointVec& vertices = poly.getVertices();

	immBegin(type, vertices.size());
	for (int i = 0; i < vertices.size(); i++) {
		immAttrib4fv(attr_color, painter.getVertexColor(i));
		immVertex2f(attr_pos, vertices[i].x, vertices[i].y);
	}
	immEnd();
}
static void stage_polygon_draw(
        const bwPainter& painter, const bwPolygon& poly,
        const bwColor& color, const PrimitiveType type,
        const unsigned int attr_pos, const unsigned int attr_color)
{
	if (painter.isGradientEnabled()) {
		stage_polygon_draw_shaded(painter, poly, type, attr_pos, attr_color);
	}
	else {
		stage_polygon_draw_uniform_color(poly, color, type, attr_pos);
	}
}

/**
 * The main polygon draw callback which is used to draw all geometry of widgets.
 */
static void stage_polygon_draw_cb(const bwPainter& painter, const bwPolygon& poly)
{
	const bool is_shaded = painter.isGradientEnabled();
	ShaderProgram shader_program(is_shaded ? ShaderProgram::ID_SMOOTH_COLOR : ShaderProgram::ID_UNIFORM_COLOR);
	bwColor color = painter.getActiveColor();
	PrimitiveType prim_type = stage_polygon_drawtype_convert(painter.active_drawtype);
	VertexFormat* format = immVertexFormat();
	unsigned int attr_pos = VertexFormat_add_attrib(format, "pos", COMP_F32, 2, KEEP_FLOAT);
	unsigned int attr_color;

	if (is_shaded) {
		attr_color = VertexFormat_add_attrib(format, "color", COMP_F32, 4, KEEP_FLOAT);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	immBindProgram(shader_program.ProgramID(), shader_program.getInterface());

	if (painter.active_drawtype == bwPainter::DRAW_TYPE_OUTLINE) {
		bwColor drawcolor = color;

		drawcolor[3] /= WIDGET_AA_JITTER;

		for (int i = 0; i < WIDGET_AA_JITTER; i++) {
			gpuTranslate2f(jit[i]);
			stage_polygon_draw(painter, poly, drawcolor, prim_type, attr_pos, attr_color);
			gpuTranslate2f(-jit[i][0], -jit[i][1]);
		}
	}
	else {
		stage_polygon_draw(painter, poly, color, prim_type, attr_pos, attr_color);
	}

	immUnbindProgram();
	glDisable(GL_BLEND);
}

static float stage_text_xpos_calc(
        Font& font, const std::string& text,
        const bwRectanglePixel& rectangle, const TextAlignment alignment)
{
	return (alignment == TEXT_ALIGN_CENTER) ?
	            (rectangle.centerX() - (font.calculateStringWidth(text) / 2.0f)) : (rectangle.xmin + 10.0f); // XXX +10 is ugly. Goes out of widget rectangle even.
}

/**
 * The main text draw callback which is used to draw all text of widgets.
 */
static void stage_text_draw_cb(
        const bwPainter &painter, const std::string& text,
        const bwRectanglePixel& rectangle, const TextAlignment alignment,
        void* text_draw_arg)
{
	Font* font = reinterpret_cast<Font*>(text_draw_arg);
	const float font_height = font->getSize();
	const float draw_pos_x = stage_text_xpos_calc(*font, text, rectangle, alignment);
	const float draw_pos_y = rectangle.centerY() - (font_height / 2.0f) + 1.0f;

	font->setActiveColor(painter.getActiveColor());
	font->render(text, draw_pos_x, draw_pos_y);
}

Stage::Stage(const unsigned int width, const unsigned int height) :
    width(width), height(height), last_hovered(nullptr)
{
	// Initialize drawing callbacks for the stage
	bwPainter::drawPolygonCb = stage_polygon_draw_cb;
	bwPainter::drawTextCb = stage_text_draw_cb;

	initFonts();
	bwPainter::text_draw_arg = font;

	bwStyleManager& style_manager = bwStyleManager::getStyleManager();
	style_manager.registerDefaultStyleTypes();
	activateStyleID(bwStyle::STYLE_BLENDER_CLASSIC);
}

Stage::~Stage()
{
	for (bwWidget* widget : widgets) {
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

void Stage::activateStyleID(bwStyle::StyleTypeID type_id)
{
	bwStyleManager& style_manager = bwStyleManager::getStyleManager();
	style = std::unique_ptr<bwStyle>(style_manager.createStyleFromTypeID(type_id));
}

void Stage::draw(unsigned int width, unsigned int height)
{
	gpuOrtho(0.0f, width, 0.0f, height);

	for (bwWidget* widget : widgets) {
		widget->draw(*style);
	}
}

void Stage::widgetAdd(bwWidget* widget)
{
	widgets.push_front(widget);
}

bwWidget* Stage::getWidgetAt(const unsigned int index)
{
	unsigned int i = 0;
	for (bwWidget* widget : widgets) {
		if (i++ == index) {
			return widget;
		}
	}

	return nullptr;
}

void Stage::handleMouseMovementEvent(const int mouse_xy[])
{
	for (bwWidget* widget : widgets) {
		if (widget->isCoordinateInside(mouse_xy[0], mouse_xy[1])) {
			if (widget != last_hovered) {
				if (last_hovered) {
					last_hovered->mouseLeave();
				}
				widget->mouseEnter();
				last_hovered = widget;
			}
		}
		else if (widget == last_hovered) {
			widget->mouseLeave();
			last_hovered = nullptr;
		}
	}
}

void Stage::handleMouseButtonEvent(
        const Window& /*win*/, const int mouse_xy[2],
        int button, int /*action*/, int /*mods*/)
{
	bwWidget::MouseButton bw_mouse_button = convertGlfwMouseButton(button);

	for (bwWidget* widget : widgets) {
		if (widget->isCoordinateInside(mouse_xy[0], mouse_xy[1])) {
			widget->onClick(bw_mouse_button);
		}
	}
}

bwWidget::MouseButton Stage::convertGlfwMouseButton(int glfw_button)
{
	switch (glfw_button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			return bwWidget::MOUSE_BUTTON_LEFT;
		case GLFW_MOUSE_BUTTON_RIGHT:
			return bwWidget::MOUSE_BUTTON_RIGHT;
	}

	// XXX randomish default value
	return bwWidget::MOUSE_BUTTON_LEFT;
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
