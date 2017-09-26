#pragma once

#include "bwWidget.h"

namespace bWidgetsDemo {

class Event
{
	friend class EventManager;

protected:
	Event() {}
};

class MouseEvent : Event
{
public:
	enum MouseEventType {
		MOUSE_EVENT_PRESS,
		MOUSE_EVENT_RELEASE,
		MOUSE_EVENT_MOVE,

		MOUSE_EVENT_UNKNOWN
	};

	MouseEvent(
	        MouseEventType type,
	        bWidgets::bwWidget::MouseButton button,
	        const bWidgets::bwPoint& location);

	bool isClick() const;

	bWidgets::bwWidget::MouseButton getButton() const;
	MouseEventType getType() const;
	int getHorizontalDragDistance() const;

	static const bWidgets::bwPoint& getMouseLocation();

private:
	MouseEventType type;
	bWidgets::bwWidget::MouseButton button;

	static bWidgets::bwPoint location;
	// Location during previous mouse button press.
	static bWidgets::bwPoint last_down_location;
	static bWidgets::bwWidget::MouseButton last_down_button;
};

} // namespace bWidgetsDemo
