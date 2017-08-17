#pragma once

#include "bwPainter.h"
#include "bwRectangle.h"

namespace bWidgets {

/**
 * \brief Abstract base class that all widgets derive from.
 */
struct bwWidget
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

	bwWidget(const WidgetType type, const bwRectanglePixel& rectangle);
	virtual ~bwWidget() {}

	virtual bool isCoordinateInside(int x, int y);

	virtual void draw(class bwStyle& style) = 0;
	virtual void onClick();
	virtual void mouseEnter();
	virtual void mouseLeave();

	void (*apply)(bwWidget& widget);

	bwRectanglePixel rectangle;
	unsigned int rounded_corners;
};

} // namespace bWidgets
