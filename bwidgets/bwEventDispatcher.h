#pragma once

namespace bWidgets {

class bwEvent;
class bwMouseButtonEvent;
namespace bwScreenGraph {
	class ScreenGraph;
	class Node;
}

/**
 * \brief Mangages sending events to screen-graph nodes based on current state.
 *
 * Based on the \link bWidgets::bwScreenGraph::ScreenGraph::context screen-graph
 * context as state, the event dispatcher sends events to the screen-graph nodes
 * in user focus (i.e. hovered or active node). That means, it calls the nodes
 * event listener corresponding to the determined event.
 */
class bwEventDispatcher
{
public:
	bwEventDispatcher(bwScreenGraph::ScreenGraph& _screen_graph) :
	    screen_graph(_screen_graph) {}

	void dispatchMouseMovement(bwEvent);
	void dispatchMouseButtonPress(bwMouseButtonEvent&);

private:
	/** Reference back to the screen-graph owning this dispatcher */
	bwScreenGraph::ScreenGraph& screen_graph;

	// XXX Temp: Public so host app can manage scrollbar hovering.
public: void changeContextHovered(bwScreenGraph::Node*);
};

} // namespace bWidgets
