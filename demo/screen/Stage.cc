// drawing
extern "C" {
#include "../../extern/gawain/gawain/immediate.h"
}
#include "ShaderProgram.h"

// bWidgets lib
#include "bwPolygon.h"
#include "bwStyleManager.h"

#include "Event.h"
#include "Font.h"
#include "GPU.h"
#include "Layout.h"
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
void Stage::PolygonDrawCb(
        const bwPainter& painter,
        const bwPolygon& poly)
{
	const bool is_shaded = painter.isGradientEnabled();
	ShaderProgram shader_program(is_shaded ? ShaderProgram::ID_SMOOTH_COLOR : ShaderProgram::ID_UNIFORM_COLOR);
	const bwColor& color = painter.getActiveColor();
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
	int value = 0;

	switch (alignment) {
		case TEXT_ALIGN_LEFT:
			value = rectangle.xmin + 10; // XXX +10 is ugly. Goes out of widget rectangle even.
			break;
		case TEXT_ALIGN_CENTER:
			value = rectangle.centerX() - (font.calculateStringWidth(text) / 2.0f);
			break;
		case TEXT_ALIGN_RIGHT:
			value = rectangle.xmax - font.calculateStringWidth(text) - 10; // XXX -10 is ugly. Goes out of widget rectangle even.
			break;
	}

	return value;
}

/**
 * The main text draw callback which is used to draw all text of widgets.
 */
void Stage::TextDrawCb(
        const bwPainter &painter,
        const std::string& text,
        const bwRectanglePixel& rectangle,
        const TextAlignment alignment)
{
	const float font_height = font->getSize();
	const float draw_pos_x = stage_text_xpos_calc(*font, text, rectangle, alignment);
	const float draw_pos_y = rectangle.centerY() - (font_height / 2.0f) + 1.0f;

	font->setActiveColor(painter.getActiveColor());
	font->setMask(painter.getContentMask());
	font->render(text, draw_pos_x, draw_pos_y);
}


std::unique_ptr<bWidgets::bwStyle> Stage::style = nullptr;
std::unique_ptr<Font> Stage::font = nullptr;
float Stage::interface_scale = 1.0f;

Stage::Stage(const unsigned int width, const unsigned int height) :
    width(width), height(height), last_hovered(nullptr), dragged_widget(nullptr)
{
	// Initialize drawing callbacks for the stage
	bwPainter::drawPolygonCb = Stage::PolygonDrawCb;
	bwPainter::drawTextCb = Stage::TextDrawCb;

	initFonts();

	bwStyleManager& style_manager = bwStyleManager::getStyleManager();
	style_manager.registerDefaultStyleTypes();
	activateStyleID(bwStyle::STYLE_BLENDER_CLASSIC);

	layout = new RootLayout(height, width);
	layout->padding = 7;
	layout->item_margin = 5;
}

Stage::~Stage()
{
	delete layout;
}

void Stage::initFonts()
{
	// Initialize freetype
	Font::initFontReading();

	// Initialize default font
	font = std::unique_ptr<Font>(Font::loadFont("bfont.ttf", RESOURCES_PATH_STR));
	font->setSize(11.0f * interface_scale);
}

void Stage::activateStyleID(bwStyle::StyleTypeID type_id)
{
	bwStyleManager& style_manager = bwStyleManager::getStyleManager();
	style = std::unique_ptr<bwStyle>(style_manager.createStyleFromTypeID(type_id));
	style->dpi_fac = interface_scale;
}

void Stage::draw(unsigned int width, unsigned int height)
{
	gpuOrtho(0.0f, width, 0.0f, height);

	layout->resolve(interface_scale);
	layout->draw(*style);
}

void Stage::setInterfaceScale(const float value)
{
	interface_scale = value;
	font->setSize(11.0f * value);
	style->dpi_fac = value;
}

struct MouseEventStageWrapper {
	Stage& stage;
	const MouseEvent& event;
};

bool Stage::handleMouseMovementWidgetCb(bwWidget& widget, void* custom_data)
{
	MouseEventStageWrapper& data = *(MouseEventStageWrapper*)custom_data;

	if (widget.isCoordinateInside(data.event.getMouseLocation())) {
		if (&widget != data.stage.last_hovered) {
			if (data.stage.last_hovered) {
				data.stage.last_hovered->mouseLeave();
			}
			widget.mouseEnter();
			data.stage.last_hovered = &widget;
		}
	}
	else if (&widget == data.stage.last_hovered) {
		widget.mouseLeave();
		data.stage.last_hovered = nullptr;
	}

	return true;
}

void Stage::handleMouseMovementEvent(const MouseEvent& event)
{
	MouseEventStageWrapper data = {*this, event};
	layout->iterateWidgets(handleMouseMovementWidgetCb, &data);
}

bool Stage::handleMouseEventWidgetCb(bwWidget& widget, void* custom_data)
{
	MouseEventStageWrapper& data = *(MouseEventStageWrapper*)custom_data;
	const MouseEvent& event = data.event;

	if (widget.isCoordinateInside(event.getMouseLocation())) {
		if (event.isClick()) {
			widget.mouseClickEvent(event.getButton());
			// TODO handlers should return value to pass on or block event.
		}

		switch (event.getType()) {
			case MouseEvent::MOUSE_EVENT_PRESS:
				widget.mousePressEvent(event.getButton());
				data.stage.dragged_widget = &widget;
				break;
			case MouseEvent::MOUSE_EVENT_RELEASE:
				widget.mouseReleaseEvent(event.getButton());
				break;
			default:
				return true;
		}

		return false;
	}

	return true;
}

void Stage::handleMouseButtonEvent(const MouseEvent& event)
{
	MouseEventStageWrapper data = {*this, event};
	layout->iterateWidgets(handleMouseEventWidgetCb, &data);
	if (event.getType() == MouseEvent::MOUSE_EVENT_RELEASE) {
		dragged_widget = nullptr;
	}
}

bool Stage::handleMouseDragWidgetCb(bwWidget& widget, void* custom_data)
{
	MouseEventStageWrapper& data = *(MouseEventStageWrapper*)custom_data;

	if (&widget == data.stage.dragged_widget) {
		widget.mouseDragEvent(data.event.getButton(), data.event.getHorizontalDragDistance());
		return false;
	}

	return true;
}

void Stage::handleMouseDragEvent(const MouseEvent& event)
{
	MouseEventStageWrapper data = {*this, event};
	layout->iterateWidgets(handleMouseDragWidgetCb, &data);
}

void Stage::handleResizeEvent(const Window& win)
{
	width = win.getWidth();
	height = win.getHeight();
	layout->setMaxSize(width);
	layout->setYmax(height);
}
