#include "bwEvent.h"

namespace bWidgets {

void bwEvent::swallow()
{
  is_swallowed = true;
}

void bwEvent::unswallow()
{
  is_swallowed = false;
}

bool bwEvent::isSwallowed() const
{
  return is_swallowed;
}

void bwMouseButtonDragEvent::addMouseMovement(bwPoint new_location)
{
  if (drag_state == DRAGGING_BELOW_THRESHOLD &&
      distance(location, new_location) > DRAG_THRESHOLD) {
    drag_state = DRAGGING;
  }

  drag_distance = new_location - location;
}

}  // namespace bWidgets
