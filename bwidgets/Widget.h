#pragma once

#include "Rectangle.h"

namespace bWidgets {

class Widget
{
public:
	enum WidgetType {
		WIDGET_TYPE_LABEL,
		WIDGET_TYPE_PUSH_BUTTON,
		WIDGET_TYPE_RADIO_BUTTON,
	} type;

	enum WidgetState {
		STATE_NORMAL,
		STATE_HIGHLIGHTED,
		STATE_SUNKEN,
	} state;

	Widget(const WidgetType type, const Rectangle<unsigned int>& rectangle);
	virtual ~Widget() {}

	bool isCoordinateInside(int x, int y);

	virtual void draw(class Style& style) = 0;
	virtual void onClick();
	virtual void mouseEnter();
	virtual void mouseLeave();

	Rectangle<unsigned int> rectangle;
};

} // namespace bWidgets
