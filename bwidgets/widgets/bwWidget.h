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

	bwWidget(
	        const WidgetType type,
	        const unsigned int width_hint = 0, const unsigned int height_hint = 0);
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

	/**
	 * Final rectangle defining the widget bounding-box.
	 * \note This really is assumed to be the final bounding-box, bwStyle.dpi_fac will not get applied to it.
	 */
	bwRectanglePixel rectangle;

	/**
	 * Define size hints for the widget. bWidgets itself doesn't do anything with it. The actual
	 * application can use it for its layout calculations or simply ignore it. For bWidgets all
	 * that matters is the final \a rectangle. Like the name suggests it's really just a hint.
	 */
	unsigned int width_hint, height_hint;
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
