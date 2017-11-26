#include <cassert>
#include <math.h>

// bWidgets lib
#include "bwPainter.h"
#include "bwRange.h"
#include "bwStyleManager.h"
#include "bwUtil.h"

#include "Event.h"
#include "Font.h"
#include "GawainPaintEngine.h"
#include "Layout.h"
#include "Window.h"

#include "Stage.h"


using namespace bWidgetsDemo;
using namespace bWidgets; // Less verbose

bwPointer<bwStyle> Stage::style = nullptr;
bwPointer<Font> Stage::font = nullptr;
float Stage::interface_scale = 1.0f;


Stage::Stage(const unsigned int width, const unsigned int height) :
    mask_width(width), mask_height(height), last_hovered(nullptr), dragged_widget(nullptr)
{
	initFonts();

	// After font-init!
	bwPainter::paint_engine = bwPointer_new<GawainPaintEngine>(*font);

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
	font = bwPointer<Font>(Font::loadFont("bfont.ttf", RESOURCES_PATH_STR));
	font->setSize(11.0f * interface_scale);
}

void Stage::activateStyleID(bwStyle::StyleTypeID type_id)
{
	bwStyleManager& style_manager = bwStyleManager::getStyleManager();
	style = bwPointer<bwStyle>(style_manager.createStyleFromTypeID(type_id));
	style->dpi_fac = interface_scale;
}

namespace bWidgetsDemo {

class ScrollbarApplyValueFunctor : public bwFunctorInterface
{
	Stage& stage;
	const bwScrollBar& scrollbar;

public:
	ScrollbarApplyValueFunctor(Stage& stage, const bwScrollBar& scrollbar) : stage(stage), scrollbar(scrollbar) {}
	inline void operator()() override
	{
		stage.setScrollValue(scrollbar.scroll_offset);
	}
};

} // namespace bWidgetsDemo


void Stage::drawScrollbars()
{
	if (isScrollable()) {
		const unsigned int padding = (unsigned int)(4 * interface_scale);

		if (!scrollbar) {
			scrollbar = bwPointer_new<bwScrollBar>(getScrollbarWidth(), mask_height);
			scrollbar->apply_functor = bwPointer_new<ScrollbarApplyValueFunctor>(*this, *scrollbar);
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
	bwRectanglePixel rect{0, (int)mask_width, 0, (int)mask_height};
	bwColor clear_color{0.447f, 0.447f, 0.447f};

	bwPainter::paint_engine->setupViewport(rect, clear_color);

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
