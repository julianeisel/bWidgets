#include "glm.hpp"

#include "bwDistance.h"

#include "EventManager.h"

using namespace bWidgetsDemo;


bWidgets::bwPoint MouseEvent::location{};
bWidgets::bwPoint MouseEvent::last_down_location{};
bWidgets::bwWidget::MouseButton MouseEvent::last_down_button = bWidgets::bwWidget::MOUSE_BUTTON_UNKNOWN;

MouseEvent::MouseEvent(
        MouseEventType type,
        bWidgets::bwWidget::MouseButton _button,
        const bWidgets::bwPoint& _location) :
    type(type)
{
	if (type == MOUSE_EVENT_PRESS) {
		last_down_location = _location;
		last_down_button = _button;
	}
	else if (type == MOUSE_EVENT_MOVE) {
		_button = last_down_button;
	}
	location = _location;
	button = _button;
}

bool MouseEvent::isClick() const
{
	if ((type == MOUSE_EVENT_RELEASE) && (button == last_down_button)) {
		const float dist = glm::distance(glm::vec2(location.x, location.y),
		                                 glm::vec2(last_down_location.x, last_down_location.y));
		if (dist <= 3) {
			return true;
		}
	}

	return false;
}

bWidgets::bwWidget::MouseButton MouseEvent::getButton() const
{
	return button;
}

MouseEvent::MouseEventType MouseEvent::getType() const
{
	return type;
}

bWidgets::bwDistance MouseEvent::getDragDistance() const
{
	EventManager& manager = EventManager::ensureEventManager();
	return manager.isDragging() ? location - last_down_location : 0;
}

const bWidgets::bwPoint& MouseEvent::getMouseLocation()
{
	return location;
}
