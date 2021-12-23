#pragma once

#include <array>
#include <functional>
#include <list>
#include <unordered_map>

#include "bwWidget.h"

#include "Node.h"

namespace bWidgets {

class bwEvent;
class bwMouseButtonEvent;
class bwMouseButtonDragEvent;
class bwMouseWheelEvent;
class bwWidget;

namespace bwScreenGraph {

class Builder;
class Node;

// TODO bwHandlingContext
using EventListener = std::function<void(Node&)>;

/**
 * \brief Event handling base-class for screen-graph nodes.
 *
 * For persistent widgets (i.e. widgets for which there is a #PersistentNodePtr registered or which
 * return true for #bwWidgetState.alwaysPersistent()), the event handler is kept persistent as
 * well. It usually stores state data needed for the handling, so it is considered part of the
 * state. See #Node::moveState().
 *
 * TODO:
 * * Only default `onFoo()` style listeners are supported, custom `addEventListener()` doesn't work
 *   yet.
 * * Many widget handlers are friend classes to the widgets to access internal data. Instead
 *   widgets should have APIs to manipulate their state anyway, which they don't have yet.
 */
class EventHandler {
  friend class bwScreenGraph::Node;

 public:
  enum EventType {
    MOUSE_ENTER,
    MOUSE_LEAVE,

    TOT_EVENT_TYPES,
  };

  EventHandler(bwScreenGraph::Node& node);
  virtual ~EventHandler() = default;

  void addEventListener(EventType event_type, EventListener listener);

  virtual void onMouseMove(bwEvent&);
  virtual void onMouseEnter(bwEvent&);
  virtual void onMouseLeave(bwEvent&);
  virtual void onMousePress(bwMouseButtonEvent&);
  virtual void onMouseRelease(bwMouseButtonEvent&);
  virtual void onMouseClick(bwMouseButtonEvent&);
  virtual void onMouseDrag(bwMouseButtonDragEvent&);
  virtual void onMouseWheel(bwMouseWheelEvent&);

 protected:
  /**
   * The screen-graph node this event-handler belongs to.
   */
  bwScreenGraph::Node& Node() const;

 private:
  /** Parent node of this handler. Non-owning, never null. */
  bwScreenGraph::Node* node;

  std::array<std::list<EventListener>, TOT_EVENT_TYPES> listeners;
  //	std::unordered_map<EventType, std::list<EventListener>> listener_map;
};

template<class WidgetType_> class WidgetEventHandler : public EventHandler {
 protected:
  using EventHandler::EventHandler;

  /**
   * The widget this event-handler belongs to (as composed by the node).
   */
  virtual WidgetType_& Widget() const
  {
    if constexpr (std::is_same<WidgetType_, bwWidget>::value) {
      return Node().Widget();
    }
    else {
      static_assert(std::is_base_of_v<bwWidget, WidgetType_>, "Should derive from bwWidget");
      assert(dynamic_cast<WidgetType_*>(Node().Widget()) != nullptr);

      /* Compiler doesn't want us to call widget_cast<>() here for some reason. */
      return *dynamic_cast<WidgetType_*>(Node().Widget());
    }
  }
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
