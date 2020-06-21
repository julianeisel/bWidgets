#pragma once

#include "bwDistance.h"
#include "bwPoint.h"

namespace bWidgets {

class bwEvent {
 public:
  bwEvent(bwPoint _location);
  virtual ~bwEvent() = default;

  void swallow();
  void unswallow();
  auto isSwallowed() const -> bool;

  /** Where did the event happen? */
  const bwPoint location;

 private:
  bool is_swallowed{false};
};

class bwMouseButtonEvent : public bwEvent {
 public:
  enum class Button {
    LEFT,
    RIGHT,
    WHEEL,

    UNKNOWN
  };

  bwMouseButtonEvent(Button _button, bwPoint _location);

  const Button button;
};

class bwMouseButtonDragEvent : public bwMouseButtonEvent {
 public:
  enum DragState {
    DRAGGING_BELOW_THRESHOLD,
    DRAGGING,
  };

  bwMouseButtonDragEvent(Button _button, bwPoint _location);

  void addMouseMovement(bwPoint new_location);

  const static short DRAG_THRESHOLD = 3;

  bwDistance drag_distance;
  DragState drag_state;
};

class bwMouseWheelEvent : public bwMouseButtonEvent {
 public:
  enum class Direction { UP, DOWN };

  bwMouseWheelEvent(Direction _direction, bwPoint _location);

  auto getDirection() const -> Direction;

 private:
  Direction direction;
};

}  // namespace bWidgets
