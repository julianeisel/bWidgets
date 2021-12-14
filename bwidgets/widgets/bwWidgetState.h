#pragma once

namespace bWidgets {

class bwWidgetState {
 public:
  virtual ~bwWidgetState() = default;

  enum BaseState {
    NORMAL = 0,
    HIGHLIGHTED,
    SUNKEN,

    STATE_TOT
  };

  BaseState base = NORMAL;
};

using WidgetBaseState = bwWidgetState::BaseState;

}  // namespace bWidgets
