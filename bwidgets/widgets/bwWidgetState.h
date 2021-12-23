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

  /**
   * bWidgets only keeps the state of active widgets, unless this is overriden to return true. For
   * convenience/simplicity, state objects can inherit from #bwWidgetStateAlwaysPersistent, which
   * overrides this to always return true.
   * Persistence is managed as part of reconstruction, see
   * #bwScreenGraph::Constructor::reconstruct().
   */
  virtual auto alwaysPersistent() const -> bool
  {
    return false;
  }
};

using WidgetBaseState = bwWidgetState::BaseState;

/**
 * Enforce a widget to always have persistent state, not only when active (or referenced by some
 * other persistent pointer). Convenience/readability helper that can be inherited from instead of
 * #bwWidgetState.
 */
class bwWidgetStateAlwaysPersistent : public bwWidgetState {
  virtual auto alwaysPersistent() const -> bool
  {
    return true;
  }
};

}  // namespace bWidgets
