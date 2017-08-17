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

	bwWidget(const WidgetType type, const bwRectanglePixel& rectangle);
	virtual ~bwWidget() {}

	virtual bool isCoordinateInside(int x, int y);
	virtual void draw(class bwStyle& style) = 0;

	bwRectanglePixel rectangle;
};

/**
 * Performs a conditional widget cast from bwWidget to derived class if valid.
 * \return the derrived widget class of type T or nullptr if cast is not valid.
 */
template<class T>
inline T widget_cast(bwWidget* widget)
{
	return dynamic_cast<T>(widget);
}
template<class T>
inline T widget_cast(const bwWidget* widget)
{
	return dynamic_cast<T>(widget);
}
template<class T>
inline T widget_cast(bwWidget& widget)
{
	return dynamic_cast<T>(widget);
}
template<class T>
inline T widget_cast(const bwWidget& widget)
{
	return dynamic_cast<T>(widget);
}

} // namespace bWidgets
