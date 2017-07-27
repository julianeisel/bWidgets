#pragma once

#include <list>
#include "Widget.h"


namespace bWidgetDemo {

class Stage {
public:
	Stage();
	~Stage();

	void draw(unsigned int width, unsigned int height);
	void widgetAdd(bWidgets::Widget* widget);

	void handleMouseButtonEvent(class Window *win, const int mouse_xy[], int button, int action, int mods);

private:
	std::list<bWidgets::Widget*> widgets;
	class Font* font;

	void initFonts();
};

} // namespace bWidgetDemo
