#pragma once

#include <list>
#include <memory>

#include "bwScrollBar.h"
#include "bwStyle.h"
#include "bwWidget.h"


using namespace bWidgets;

namespace bWidgetsDemo {

class Stage {
public:
	Stage(const unsigned int mask_width, const unsigned int mask_height);
	virtual ~Stage();

	void draw();

	void handleMouseMovementEvent(
	        const class MouseEvent& event);
	void handleMouseButtonEvent(
	        const class MouseEvent& event);
	void handleMouseDragEvent(
	        const class MouseEvent& event);
	void handleMouseScrollEvent(
	        const class MouseEvent& event);

	virtual void handleWindowResizeEvent(const class Window& win);

protected:
	class RootLayout* layout;

	// Static members, global UI data for all stages
	static std::unique_ptr<bwStyle> style;
	static std::unique_ptr<class Font> font;
	static float interface_scale;

	unsigned int mask_width, mask_height;
	int vert_scroll = 0;

	static void TextDrawCb(
	        const class bwPainter& painter,
	        const std::string& text,
	        const bwRectanglePixel& rectangle,
	        const TextAlignment alignment);
	static void PolygonDrawCb(
	        const class bwPainter& painter,
	        const class bwPolygon& poly);

	void activateStyleID(bwStyle::StyleTypeID type_id);
	static void setInterfaceScale(const float value);

private:
	// Not part of the layout yet. We'd need to support horizontal root layouts first.
	std::unique_ptr<bwScrollBar> scrollbar;
	// The lastly hovered widget. Stored to detect mouse-leave events without lookups.
	bwWidget* last_hovered;
	bwWidget* dragged_widget;

	void initFonts();
	void drawScrollbars();
	void updateContentBounds();
	void validizeScrollValue();
	bwWidget*findWidgetAt(const bwPoint& coordinate);
	void updateWidgetHovering(
	        const MouseEvent&,
	        bwWidget&);
	bool handleWidgetMouseButtonEvent(
	        const MouseEvent&,
	        bwWidget&);

	static void applyScrollbarValueCb(bwWidget& widget);

	bool isScrollable() const;
	unsigned int getScrollbarWidth() const;
	unsigned int getContentWidth() const;
	unsigned int getContentHeight() const;
	void setScrollValue(int value);
};

} // namespace bWidgetsDemo
