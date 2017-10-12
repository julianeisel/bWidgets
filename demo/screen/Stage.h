#pragma once

#include <list>
#include <memory>

#include "bwStyle.h"
#include "bwWidget.h"


namespace bWidgetsDemo {

class Stage {
public:
	Stage(const unsigned int width, const unsigned int height);
	virtual ~Stage();

	void draw(unsigned int width, unsigned int height);

	void activateStyleID(bWidgets::bwStyle::StyleTypeID type_id);

	void handleMouseMovementEvent(
	        const class MouseEvent& event);
	void handleMouseButtonEvent(
	        const class MouseEvent& event);
	void handleMouseDragEvent(
	        const class MouseEvent& event);
	void handleMouseScrollEvent(
	        const class MouseEvent& event);

	virtual void handleResizeEvent(const class Window& win);

protected:
	class RootLayout* layout;

	// Static members, global UI data for all stages
	static std::unique_ptr<bWidgets::bwStyle> style;
	static std::unique_ptr<class Font> font;
	static float interface_scale;

	unsigned int width, height;
	int vert_scroll = 0;

	static void TextDrawCb(
	        const bWidgets::bwPainter &painter,
	        const std::string& text,
	        const bWidgets::bwRectanglePixel& rectangle,
	        const bWidgets::TextAlignment alignment);
	static void PolygonDrawCb(
	        const bWidgets::bwPainter& painter,
	        const bWidgets::bwPolygon& poly);

	static void setInterfaceScale(const float value);

private:
	// The lastly hovered widget. Stored to detect mouse-leave events without lookups.
	bWidgets::bwWidget* last_hovered;
	bWidgets::bwWidget* dragged_widget;

	void initFonts();

	static bool handleMouseMovementWidgetCb(bWidgets::bwWidget&, void*);
	static bool handleMouseEventWidgetCb(bWidgets::bwWidget&, void*);
	static bool handleMouseDragWidgetCb(bWidgets::bwWidget&, void*);

	bool isScrollable() const;
	unsigned int getContentHeight() const;
};

} // namespace bWidgetsDemo
