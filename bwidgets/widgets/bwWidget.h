#pragma once

#include "bwPainter.h"
#include "bwRectangle.h"

namespace bWidgets {

/**
 * \brief Abstract base class that all widgets derive from.
 */
class bwWidget
{
public:
	enum WidgetType {
		WIDGET_TYPE_LABEL,
		WIDGET_TYPE_PUSH_BUTTON,
		WIDGET_TYPE_RADIO_BUTTON,
		WIDGET_TYPE_TEXT_BOX,
		WIDGET_TYPE_NUMBER_SLIDER,

		WIDGET_TYPE_TOT
	} type;

	enum WidgetState {
		STATE_NORMAL,
		STATE_HIGHLIGHTED,
		STATE_SUNKEN,

		STATE_TOT
	} state;


	// XXX temporarily placed here.
	enum MouseButton {
		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_RIGHT,

		MOUSE_BUTTON_UNKNOWN,
	};

	bwWidget(const WidgetType type, const bwRectanglePixel& rectangle);
	virtual ~bwWidget() {}

	virtual bool isCoordinateInside(const bwPoint& point) const;

	virtual void draw(class bwStyle& style) const = 0;

	// Events
	virtual void mousePressEvent(const MouseButton button);
	virtual void mouseReleaseEvent(const MouseButton button);
	virtual void mouseClickEvent(const MouseButton button);
	virtual void mouseDragEvent(const MouseButton button, const int drag_distance);
	virtual void mouseEnter();
	virtual void mouseLeave();

	bwRectanglePixel rectangle;
};

/**
 * Performs a conditional widget cast from bwWidget to derived class if valid.
 * \return the derrived widget class of type T or nullptr if cast is not valid.
 */
template<class T>
inline T widget_cast(bwWidget* widget)
{
	try {
		return dynamic_cast<T>(widget);
	}
	catch (const std::bad_cast&) {
		return nullptr;
	}
}
template<class T>
inline T widget_cast(const bwWidget* widget)
{
	try {
		return dynamic_cast<T>(widget);
	}
	catch (const std::bad_cast&) {
		return nullptr;
	}
}

} // namespace bWidgets
