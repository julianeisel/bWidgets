#pragma once

#include <functional>
#include <unordered_map>
#include <list>

namespace bWidgets {

class bwMouseButtonEvent;
class bwMouseButtonDragEvent;

namespace bwScreenGraph {

class Node;

// TODO bwHandlingContext
using EventListener = std::function<void(Node&)>;

/**
 * \brief API for registering and calling event-listeners
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

  virtual void onMouseEnter() = 0;
  virtual void onMouseLeave() = 0;
  virtual void onMousePress(bwMouseButtonEvent&) = 0;
  virtual void onMouseRelease(bwMouseButtonEvent&) = 0;
  virtual void onMouseClick(bwMouseButtonEvent&) = 0;
  virtual void onMouseDrag(bwMouseButtonDragEvent&) = 0;

 private:
  std::array<std::list<EventListener>, TOT_EVENT_TYPES> listeners;
  //	std::unordered_map<EventType, std::list<EventListener>> listener_map;
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
