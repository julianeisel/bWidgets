// drawing
extern "C" {
#include "../../extern/gawain/gawain/immediate.h"
}
#include "ShaderProgram.h"

// bWidgets lib
#include "bwPainter.h"
#include "bwPolygon.h"
#include "bwRange.h"
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
    mask_width(width), mask_height(height), last_hovered(nullptr), dragged_widget(nullptr)
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

void Stage::applyScrollbarValueCb(bwWidget& widget)
{
	bwScrollBar& scrollbar = *widget_cast<bwScrollBar*>(&widget);
	Stage& stage = *static_cast<Stage*>(scrollbar.custom_data);
	stage.setScrollValue(scrollbar.scroll_offset);
}

void Stage::drawScrollbars()
{
	if (isScrollable()) {
		const unsigned int padding = (unsigned int)(4 * interface_scale);

		if (!scrollbar) {
			scrollbar = std::unique_ptr<bwScrollBar>(new bwScrollBar(getScrollbarWidth(), mask_height));
			scrollbar->apply = applyScrollbarValueCb;
			scrollbar->custom_data = this;
		}

		scrollbar->rectangle = bwRectanglePixel(
		                           getContentWidth(), mask_width - padding,
		                           padding, mask_height - padding);
		scrollbar->ratio = mask_height / (float)getContentHeight();
		scrollbar->scroll_offset = vert_scroll;
		scrollbar->draw(*style);
	}
	else if (scrollbar) {
		scrollbar = nullptr;
	}
}

void Stage::draw()
{
	gpuOrtho(0.0f, mask_width, 0.0f, mask_height);

	updateContentBounds();

	layout->resolve(vert_scroll, interface_scale);
	layout->draw(*style);
	drawScrollbars();
}

void Stage::setInterfaceScale(const float value)
{
	if (value != interface_scale) {
		interface_scale = value;
		font->setSize(11.0f * value);
		style->dpi_fac = value;
	}
}

void Stage::updateContentBounds()
{
	layout->setMaxSize(getContentWidth());
	layout->setYmax(mask_height);
}

void Stage::validizeScrollValue()
{
	if (isScrollable()) {
		bwRange<int>::clampValue(vert_scroll, mask_height - getContentHeight(), 0);
	}
}

void Stage::setScrollValue(int value)
{
	vert_scroll = value;
	validizeScrollValue();
}

bwWidget* Stage::findWidgetAt(const bwPoint& coordinate)
{
	if (coordinate.x > getContentWidth()) {
		if (scrollbar) {
			return static_cast<bwWidget*>(scrollbar.get());
		}
		return nullptr;
	}

	struct WidgetLookupData {
		bwPoint coordinate;
		bwWidget* result = nullptr;
	} lookup_data;

	lookup_data.coordinate = coordinate;

	layout->iterateWidgets([](bwWidget& widget, void* customdata){
		WidgetLookupData* lookup_data = static_cast<WidgetLookupData*>(customdata);
		return widget.isCoordinateInside(lookup_data->coordinate) ? (lookup_data->result = &widget, false) : true;
	}, &lookup_data);

	return lookup_data.result;
}

void Stage::updateWidgetHovering(
        const MouseEvent& event,
        bwWidget& widget)
{
	if (widget.isCoordinateInside(event.getMouseLocation())) {
		if (&widget != last_hovered) {
			if (last_hovered) {
				last_hovered->mouseLeave();
			}
			widget.mouseEnter();
			last_hovered = &widget;
		}
	}
	else if (&widget == last_hovered) {
		widget.mouseLeave();
		last_hovered = nullptr;
	}
}

/**
 * \return True if handled successfully.
 */
bool Stage::handleWidgetMouseButtonEvent(
        const MouseEvent& event,
        bwWidget& widget)
{
	const bwPoint& location = event.getMouseLocation();

	if (event.isClick()) {
		widget.mouseClickEvent(event.getButton(), location);
		// TODO handlers should return value to pass on or block event.
	}

	switch (event.getType()) {
		case MouseEvent::MOUSE_EVENT_PRESS:
			assert(widget.isCoordinateInside(location));
			widget.mousePressEvent(event.getButton(), location);
			dragged_widget = &widget;
			break;
		case MouseEvent::MOUSE_EVENT_RELEASE:
			widget.mouseReleaseEvent(event.getButton(), location);
			dragged_widget = nullptr;
			break;
		default:
			return false;
	}

	return false;
}

void Stage::handleMouseMovementEvent(
        const MouseEvent& event)
{
	const bwPoint& mouse_location = event.getMouseLocation();

	if (bwWidget* hovered = findWidgetAt(mouse_location)) {
		updateWidgetHovering(event, *hovered);
	}
}

void Stage::handleMouseButtonEvent(
        const MouseEvent& event)
{
	// Dragged widget has priority.
	bwWidget* widget = dragged_widget ? dragged_widget : findWidgetAt(event.getMouseLocation());
	if (widget) {
		handleWidgetMouseButtonEvent(event, *widget);
	}
}

void Stage::handleMouseDragEvent(
        const MouseEvent& event)
{
	if (dragged_widget) {
		dragged_widget->mouseDragEvent(event.getButton(), event.getDragDistance());
	}
}

void Stage::handleMouseScrollEvent(
        const MouseEvent& event)
{
	if (isScrollable()) {
		char direction_fac = 0;

		if (event.getType() == MouseEvent::MOUSE_EVENT_SCROLL_DOWN) {
			direction_fac = -1;
		}
		else if (event.getType() == MouseEvent::MOUSE_EVENT_SCROLL_UP) {
			direction_fac = 1;
		}

		setScrollValue(vert_scroll + (direction_fac * 40));
	}
}

void Stage::handleWindowResizeEvent(const Window& win)
{
	mask_width = win.getWidth();
	mask_height = win.getHeight();
	validizeScrollValue();
}

unsigned int Stage::getScrollbarWidth() const
{
	return std::round(17 * interface_scale);
}

unsigned int Stage::getContentWidth() const
{
	const bool has_scrollbars = isScrollable();
	return std::max(mask_width - (has_scrollbars ? getScrollbarWidth() : 0), 0u);
}

unsigned int Stage::getContentHeight() const
{
	return layout->getHeight() + (2 * layout->padding); // TODO Padding should actually be added to layout width/height
}

bool Stage::isScrollable() const
{
	return mask_height < getContentHeight();
}
