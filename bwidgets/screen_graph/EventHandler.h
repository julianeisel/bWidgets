#pragma once

#include <functional>
#include <unordered_map>
#include <list>

namespace bWidgets {

class bwEvent;
class bwMouseButtonEvent;
class bwMouseButtonDragEvent;
class bwMouseWheelEvent;

namespace bwScreenGraph {

class Node;

// TODO bwHandlingContext
using EventListener = std::function<void(Node&)>;

/**
 * \brief API for registering and calling event-listeners
 *
 * TODO:
 * * Only default `onFoo()` style listeners are supported, custom `addEventListener()` doesn't work
 *   yet.
 * * Many widget handlers are friend classes to the widgets to access internal data. Instead
 *   widgets should have APIs to manipulate their state anyway, which they don't have yet.
 */
class EventHandler {
 public:
  EventHandler() = default;
  virtual ~EventHandler() = default;

  enum EventType {
    MOUSE_ENTER,
    MOUSE_LEAVE,

    TOT_EVENT_TYPES,
  };

  void addEventListener(EventType event_type, EventListener listener);

  virtual void onMouseMove(bwEvent&);
  virtual void onMouseEnter(bwEvent&);
  virtual void onMouseLeave(bwEvent&);
  virtual void onMousePress(bwMouseButtonEvent&);
  virtual void onMouseRelease(bwMouseButtonEvent&);
  virtual void onMouseClick(bwMouseButtonEvent&);
  virtual void onMouseDrag(bwMouseButtonDragEvent&);
  virtual void onMouseWheel(bwMouseWheelEvent&);

 private:
  std::array<std::list<EventListener>, TOT_EVENT_TYPES> listeners;
  //	std::unordered_map<EventType, std::list<EventListener>> listener_map;
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
