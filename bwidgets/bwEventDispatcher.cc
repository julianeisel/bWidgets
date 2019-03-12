#include "bwEvent.h"
#include "screen_graph/Node.h"

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

void bwEventDispatcher::handleMouseButtonPressEvent(bwMouseButtonEvent& event, Node& node)
{
	Node* hovered = findHoveredNode(event, node);

	if (hovered && hovered->eventHandler()) {
		hovered->eventHandler()->onMousePress(event);
	}
}
