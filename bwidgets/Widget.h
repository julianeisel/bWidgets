#pragma once

#include "Painter.h"
#include "Rectangle.h"

namespace bWidgets {

/**
 * \brief Abstract base class that all widgets derive from.
 */
struct Widget
{
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

	virtual bool isCoordinateInside(int x, int y);

	virtual void draw(class Style& style) = 0;
	virtual void onClick();
	virtual void mouseEnter();
	virtual void mouseLeave();

	void (*apply)(Widget& widget);

	Rectangle<unsigned int> rectangle;
	unsigned int rounded_corners;
};

} // namespace bWidgets
