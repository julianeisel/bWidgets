#include <iostream>

#include "bwEvent.h"
#include "screen_graph/Node.h"
#include "screen_graph/ScreenGraph.h"

#include "bwEventDispatcher.h"

using namespace bWidgets;
using namespace bwScreenGraph;

bwEventDispatcher::bwEventDispatcher(ScreenGraph& _screen_graph)
    : screen_graph(_screen_graph), context(_screen_graph.context)
{
}

static Node* findHoveredNode(bwEvent& event, Node& node)
{
  if (!node.Children()) {
    return nullptr;
  }

  for (auto& iter_node : *node.Children()) {
    if (iter_node->isVisible() &&
        iter_node->Rectangle().isCoordinateInside(event.location.x, event.location.y)) {
      if (Node* found_children = findHoveredNode(event, *iter_node)) {
        return found_children;
      }
      return iter_node.get();
    }
  }

  return nullptr;
}

void bwEventDispatcher::dispatchMouseMovement(bwEvent event)
{
  if (drag_event) {
    drag_event->addMouseMovement(event.location);
  }

  if (Node* active = context.active) {
    if (isDragging() && context.active->eventHandler()) {
      active->eventHandler()->onMouseDrag(drag_event.value());
    }
  }
  else {
    Node* new_hovered = findHoveredNode(event, screen_graph.Root());
    changeContextHovered(new_hovered);
  }
}

void bwEventDispatcher::dispatchMouseButtonPress(bwMouseButtonEvent& event)
{
  Node* node = context.active ? context.active : findHoveredNode(event, screen_graph.Root());

  if (node && node->eventHandler()) {
    if (node->eventHandler()) {
      node->eventHandler()->onMousePress(event);
    }
  }
  drag_event.emplace(event.button, event.location);

  if (!context.active) {
    context.active = node;
  }
}

void bwEventDispatcher::dispatchMouseButtonRelease(bwMouseButtonEvent& event)
{
  if (!context.active) {
    /* pass */
  }
  else if (EventHandler* handler = context.active->eventHandler()) {
    handler->onMouseRelease(event);
  }

  drag_event = nullopt;
  context.active = nullptr;
}

bool bwEventDispatcher::isDragging()
{
  return drag_event && (drag_event->drag_state == bwMouseButtonDragEvent::DRAGGING);
}

/**
 * Make \a new_hovered the new hovered widget, executing the onMouseEnter() and
 * onMouseLeave() listeners as needed.
 */
void bwEventDispatcher::changeContextHovered(Node* new_hovered)
{
  bwWidget* old_hovered = context.hovered;

  if (new_hovered && (old_hovered == new_hovered->Widget())) {
    return;
  }

  if (old_hovered) {
    old_hovered->onMouseLeave();
  }
  if (new_hovered && new_hovered->eventHandler()) {
    new_hovered->eventHandler()->onMouseEnter();
  }
  context.hovered = new_hovered ? new_hovered->Widget() : nullptr;
}
