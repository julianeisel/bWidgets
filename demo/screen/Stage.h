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

	void handleMouseMovementEvent(const int mouse_xy[]);
	void handleMouseButtonEvent(const class Window& win, const int mouse_xy[], int button, int action, int mods);
	bWidgets::bwWidget::MouseButton convertGlfwMouseButton(int glfw_button);

	virtual void handleResizeEvent(const class Window& win);

	unsigned int getWidth() const; unsigned int getHeight() const;

protected:
	std::unique_ptr<bWidgets::bwStyle> style;
	std::list<bWidgets::bwWidget*> widgets;
	class Font* font;

private:
	unsigned int width, height;
	// The lastly hovered widget. Stored to detect mouse-leave events without lookups.
	bWidgets::bwWidget* last_hovered;

	void initFonts();
};

} // namespace bWidgetsDemo
