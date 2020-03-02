#include <iostream>

#include "bwEvent.h"
#include "screen_graph/EventHandler.h"
#include "screen_graph/Node.h"
#include "screen_graph/ScreenGraph.h"

#include "bwEventDispatcher.h"

using namespace bWidgets::bwScreenGraph;

namespace bWidgets {

bwEventDispatcher::bwEventDispatcher(ScreenGraph& _screen_graph)
    : screen_graph(_screen_graph), context(_screen_graph.context)
{
}

template<typename... _Args> using HandlerFunc = void (bwScreenGraph::EventHandler::*)(_Args&&...);

template<typename... _Args>
static void bubbleEvent(const bwEvent& event,
                        const Node& from_node,
                        HandlerFunc<_Args&&...> handler_func,
                        _Args&&... __args)
{
  for (const Node* node = &from_node; node && !event.isSwallowed(); node = node->Parent()) {
    if (EventHandler* handler = node->eventHandler()) {
      (handler->*handler_func)(std::forward<_Args>(__args)...);
    }
  }
}

static Node* findHoveredNode(const bwEvent& event, Node& node)
{
  const bool is_hovered = node.isVisible() &&
                          node.Rectangle().isCoordinateInside(event.location.x, event.location.y);

  if (node.Children() && is_hovered) {
    for (auto& iter_child : *node.Children()) {
      if (Node* found_child = findHoveredNode(event, *iter_child)) {
        return found_child;
      }
    }
  }

  return is_hovered ? &node : nullptr;
}

void bwEventDispatcher::dispatchMouseMovement(bwEvent event)
{
  if (drag_event) {
    drag_event->addMouseMovement(event.location);
  }

  if (Node* active = context.active) {
    if (isDragging()) {
      bubbleEvent<bwMouseButtonDragEvent&>(
          event, *active, &EventHandler::onMouseDrag, drag_event.value());
    }
  }
  else {
    Node* new_hovered = findHoveredNode(event, screen_graph.Root());

    if (new_hovered && (new_hovered == context.hovered)) {
      bubbleEvent<bwEvent&>(event, *new_hovered, &EventHandler::onMouseMove, event);
    }
    changeContextHovered(new_hovered, event);
  }
}

void bwEventDispatcher::dispatchMouseButtonPress(bwMouseButtonEvent& event)
{
  Node* node = context.active ? context.active : findHoveredNode(event, screen_graph.Root());

  if (node) {
    bubbleEvent<bwMouseButtonEvent&>(event, *node, &EventHandler::onMousePress, event);
  }
  drag_event.emplace(event.button, event.location);

  if (!context.active) {
    context.active = node;
  }
}

void bwEventDispatcher::dispatchMouseButtonRelease(bwMouseButtonEvent& event)
{
  if (context.active) {
    bubbleEvent<bwMouseButtonEvent&>(event, *context.active, &EventHandler::onMouseRelease, event);

    if (!isDragging()) {
      /* Even if the drag event was already sent, we may also need to send the click event, so
       * unswallow it for that purpose. */
      event.unswallow();

      bubbleEvent<bwMouseButtonEvent&>(event, *context.active, &EventHandler::onMouseClick, event);
    }
  }

  drag_event = nullopt;
  context.active = nullptr;
}

void bwEventDispatcher::dispatchMouseWheelScroll(bwMouseWheelEvent& event)
{
  if (context.hovered) {
    bubbleEvent<bwMouseWheelEvent&>(event, *context.hovered, &EventHandler::onMouseWheel, event);
  }
}

bool bwEventDispatcher::isDragging()
{
  return drag_event && (drag_event->drag_state == bwMouseButtonDragEvent::DRAGGING);
}

/**
 * Make \a new_hovered the new hovered widget, executing the onMouseEnter() and
 * onMouseLeave() listeners as needed.
 */
void bwEventDispatcher::changeContextHovered(Node* new_hovered, bwEvent& event)
{
  Node* old_hovered = context.hovered;

  if (new_hovered && (old_hovered == new_hovered)) {
    return;
  }

  if (old_hovered) {
    bubbleEvent<bwEvent&>(event, *old_hovered, &EventHandler::onMouseLeave, event);
  }

  if (new_hovered) {
    bubbleEvent<bwEvent&>(event, *new_hovered, &EventHandler::onMouseEnter, event);
  }

  context.hovered = new_hovered;
}

}  // namespace bWidgets
