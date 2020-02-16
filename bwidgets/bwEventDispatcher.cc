#include <iostream>

#include "bwEvent.h"
#include "screen_graph/EventHandler.h"
#include "screen_graph/Node.h"
#include "screen_graph/ScreenGraph.h"

#include "bwEventDispatcher.h"

using namespace bWidgets;
using namespace bwScreenGraph;

bwEventDispatcher::bwEventDispatcher(ScreenGraph& _screen_graph)
    : screen_graph(_screen_graph), context(_screen_graph.context)
{
}

void bwEventDispatcher::bubbleEvent(const bwEvent& event,
                                    const Node& from_node,
                                    const EventBubbleFunctor functor)
{
  for (const Node* node = &from_node; node && !event.isSwallowed(); node = node->Parent()) {
    if (EventHandler* handler = node->eventHandler()) {
      functor(*node, *handler);
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
      bubbleEvent(event, *active, [this](const Node&, EventHandler& handler) {
        handler.onMouseDrag(drag_event.value());
      });
    }
  }
  else {
    Node* new_hovered = findHoveredNode(event, screen_graph.Root());

    if (new_hovered && (new_hovered == context.hovered)) {
      bubbleEvent(event, *new_hovered, [&event](const Node&, EventHandler& handler) {
        handler.onMouseMove(event);
      });
    }
    changeContextHovered(new_hovered, event);
  }
}

void bwEventDispatcher::dispatchMouseButtonPress(bwMouseButtonEvent& event)
{
  Node* node = context.active ? context.active : findHoveredNode(event, screen_graph.Root());

  if (node) {
    bubbleEvent(event, *node, [&event](const Node&, EventHandler& handler) {
      handler.onMousePress(event);
    });
  }
  drag_event.emplace(event.button, event.location);

  if (!context.active) {
    context.active = node;
  }
}

void bwEventDispatcher::dispatchMouseButtonRelease(bwMouseButtonEvent& event)
{
  if (context.active) {
    bubbleEvent(event, *context.active, [&event](const Node&, EventHandler& handler) {
      handler.onMouseRelease(event);
    });

    if (!isDragging()) {
      /* Even if the drag event was already sent, we may also need to send the click event, so
       * unswallow it for that purpose. */
      event.unswallow();

      bubbleEvent(event, *context.active, [&event](const Node&, EventHandler& handler) {
        handler.onMouseClick(event);
      });
    }
  }

  drag_event = nullopt;
  context.active = nullptr;
}

void bwEventDispatcher::dispatchMouseWheelScroll(bwMouseWheelEvent& event)
{
  if (context.hovered) {
    bubbleEvent(event, *context.hovered, [&event](const Node&, EventHandler& handler) {
      handler.onMouseWheel(event);
    });
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
    bubbleEvent(event, *old_hovered, [&event](const Node&, EventHandler& handler) {
      handler.onMouseLeave(event);
    });
  }

  if (new_hovered) {
    bubbleEvent(event, *new_hovered, [&event](const Node&, EventHandler& handler) {
      handler.onMouseEnter(event);
    });
  }

  context.hovered = new_hovered;
}
