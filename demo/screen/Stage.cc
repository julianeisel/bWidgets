#include <iostream>

#include "Widget.h"

#include "GPU.h"

#include "Stage.h"

#include "Window.h"


using namespace bWidgetDemo;

Stage::~Stage()
{
	for (bWidgets::Widget* widget : widgets) {
		delete widget;
	}
}

void Stage::draw(unsigned int width, unsigned int height)
{
	gpuOrtho(0.0f, width, 0.0f, height);

	for (bWidgets::Widget* widget : widgets) {
		widget->draw();
	}
}

void Stage::widgetAdd(bWidgets::Widget* widget)
{
	widgets.push_front(widget);
}

void Stage::handleMouseButtonEvent(Window* win, const int mouse_xy[2], int button, int action, int mods)
{
	for (bWidgets::Widget* widget : widgets) {
		if (widget->isCoordinateInside(mouse_xy[0], mouse_xy[1])) {
			widget->onClick();
		}
	}
}
