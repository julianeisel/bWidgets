#pragma once

#include "bwDistance.h"
#include "bwPoint.h"

namespace bWidgets {

class bwEvent {
 public:
  bwEvent(bwPoint _location) : location(_location)
  {
  }

  // Where did the event happen?
  const bwPoint location;
  // TODO
  //	void swallow();
};

class bwMouseButtonEvent : public bwEvent {
 public:
  enum MouseButton {
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_WHEEL,

    BUTTON_UNKNOWN
  };

  bwMouseButtonEvent(MouseButton _button, bwPoint _location) : bwEvent(_location), button(_button)
  {
  }

  const MouseButton button;
};

class bwMouseButtonDragEvent : public bwMouseButtonEvent {
 public:
  enum DragState {
    DRAGGING_BELOW_THRESHOLD,
    DRAGGING,
  };

  bwMouseButtonDragEvent(MouseButton _button, bwPoint _location)
      : bwMouseButtonEvent(_button, _location), drag_state(DRAGGING_BELOW_THRESHOLD)
  {
  }

  void addMouseMovement(bwPoint new_location);

  const static short DRAG_THRESHOLD = 3;

  bwDistance drag_distance;
  DragState drag_state;
};

}  // namespace bWidgets
