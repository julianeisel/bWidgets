#include "bwEvent.h"

namespace bWidgets {

bwEvent::bwEvent(bwPoint _location) : location(_location)
{
}

void bwEvent::swallow()
{
  is_swallowed = true;
}

void bwEvent::unswallow()
{
  is_swallowed = false;
}

auto bwEvent::isSwallowed() const -> bool
{
  return is_swallowed;
}

bwMouseButtonEvent::bwMouseButtonEvent(Button _button, bwPoint _location)
    : bwEvent(_location), button(_button)
{
}

bwMouseButtonDragEvent::bwMouseButtonDragEvent(Button _button, bwPoint _location)
    : bwMouseButtonEvent(_button, _location), drag_state(DRAGGING_BELOW_THRESHOLD)
{
}

void bwMouseButtonDragEvent::addMouseMovement(bwPoint new_location)
{
  if (drag_state == DRAGGING_BELOW_THRESHOLD &&
      distance(location, new_location) > DRAG_THRESHOLD) {
    drag_state = DRAGGING;
  }

  drag_distance = new_location - location;
}

bwMouseWheelEvent::bwMouseWheelEvent(Direction _direction, bwPoint _location)
    : bwMouseButtonEvent(Button::WHEEL, _location), direction(_direction)
{
}

auto bwMouseWheelEvent::getDirection() const -> Direction
{
  return direction;
}

}  // namespace bWidgets
