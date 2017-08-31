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

	void widgetAdd(bWidgets::bwWidget* widget);
	bWidgets::bwWidget* getWidgetAt(const unsigned int index);

	void handleMouseMovementEvent(const class MouseEvent& event);
	void handleMouseButtonEvent(const class MouseEvent& event);
	void handleMouseDragEvent(const MouseEvent& event);

	virtual void handleResizeEvent(const class Window& win);

protected:
	std::unique_ptr<bWidgets::bwStyle> style;
	std::list<bWidgets::bwWidget*> widgets;
	class RootLayout* layout;
	class Font* font;

	unsigned int width, height;

private:
	// The lastly hovered widget. Stored to detect mouse-leave events without lookups.
	bWidgets::bwWidget* last_hovered;
	bWidgets::bwWidget* dragged_widget;

	void initFonts();
	static bool handleMouseMovementWidgetCb(bWidgets::bwWidget&, void*);
	static bool handleMouseEventWidgetCb(bWidgets::bwWidget&, void*);
	static bool handleMouseDragWidgetCb(bWidgets::bwWidget&, void*);
};

} // namespace bWidgetsDemo
