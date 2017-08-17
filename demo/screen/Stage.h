#pragma once

#include <list>
#include <memory>

#include "Style.h"
#include "Widget.h"


namespace bWidgetDemo {

class Stage {
public:
	Stage(const unsigned int width, const unsigned int height);
	virtual ~Stage();

	void draw(unsigned int width, unsigned int height);

	void activateStyleID(bWidgets::Style::StyleTypeID type_id);

	void widgetAdd(bWidgets::Widget* widget);
	bWidgets::Widget* getWidgetAt(const unsigned int index);

	void handleMouseMovementEvent(const int mouse_xy[]);
	void handleMouseButtonEvent(const class Window& win, const int mouse_xy[], int button, int action, int mods);

	virtual void handleResizeEvent(const class Window& win);

	unsigned int getWidth() const; unsigned int getHeight() const;

protected:
	std::unique_ptr<bWidgets::Style> style;
	std::list<bWidgets::Widget*> widgets;
	class Font* font;

private:

	unsigned int width, height;

	void initFonts();
};

} // namespace bWidgetDemo
