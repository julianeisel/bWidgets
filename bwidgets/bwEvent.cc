#include "bwEvent.h"

using namespace bWidgets;

void bwMouseButtonDragEvent::addMouseMovement(bwPoint new_location)
{
  if (drag_state == DRAGGING_BELOW_THRESHOLD &&
      distance(location, new_location) > DRAG_THRESHOLD) {
    drag_state = DRAGGING;
  }

  drag_distance = new_location - location;
}
