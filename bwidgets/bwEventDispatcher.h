#pragma once

namespace bWidgets {

class bwMouseButtonEvent;
namespace bwScreenGraph {
	class Node;
}

class bwEventDispatcher
{
public:
	static void handleMouseButtonPressEvent(bwMouseButtonEvent &event, bwScreenGraph::Node &node);
};

} // namespace bWidgets
