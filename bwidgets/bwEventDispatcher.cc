#include <iostream>

#include "bwEvent.h"
#include "screen_graph/Node.h"
#include "screen_graph/ScreenGraph.h"

#include "bwEventDispatcher.h"

using namespace bWidgets;
using namespace bwScreenGraph;


static Node* findHoveredNode(bwEvent& event, Node& node)
{
	if (!node.Children()) {
		return nullptr;
	}

	for (auto& iter_node : *node.Children()) {
		if (iter_node->Rectangle().isCoordinateInside(event.location.x, event.location.y)) {
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
	Node* new_hovered = findHoveredNode(event, screen_graph.Root());
	changeHovered(new_hovered);
}

void bwEventDispatcher::dispatchMouseButtonPress(bwMouseButtonEvent& event)
{
	Node* hovered = findHoveredNode(event, screen_graph.Root());

	if (hovered && hovered->eventHandler()) {
		hovered->eventHandler()->onMousePress(event);
	}
}

/**
 * Make \a new_hovered the new hovered widget, executing the onMouseEnter() and
 * onMouseLeave() listeners as needed.
 */
void bwEventDispatcher::changeHovered(Node* new_hovered)
{
	bwWidget* old_hovered = screen_graph.context.hovered;

	if (new_hovered && (old_hovered == new_hovered->Widget())) {
		return;
	}

	if (old_hovered) {
		old_hovered->onMouseLeave();
	}
	if (new_hovered && new_hovered->eventHandler()) {
		new_hovered->eventHandler()->onMouseEnter();
	}
	screen_graph.context.hovered = new_hovered ? new_hovered->Widget() : nullptr;
}
