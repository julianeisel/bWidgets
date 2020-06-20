#pragma once

#include <optional>

#include "bwEvent.h"
#include "bwPoint.h"

namespace bWidgets {

struct bwContext;
namespace bwScreenGraph {
class ScreenGraph;
class Node;
}  // namespace bwScreenGraph

/**
 * \brief Mangages sending events to screen-graph nodes based on current state.
 *
 * Based on the \link bWidgets::bwScreenGraph::ScreenGraph::context screen-graph
 * context as state, the event dispatcher sends events to the screen-graph nodes
 * in user focus (i.e. hovered or active node). That means, it calls the nodes
 * event listener corresponding to the determined event.
 */
class bwEventDispatcher {
 public:
  bwEventDispatcher(bwScreenGraph::ScreenGraph& _screen_graph);

  void dispatchMouseMovement(bwEvent);
  void dispatchMouseButtonPress(bwMouseButtonEvent&);
  void dispatchMouseButtonRelease(bwMouseButtonEvent&);
  void dispatchMouseWheelScroll(bwMouseWheelEvent&);

 private:
  /** Reference back to the screen-graph owning this dispatcher */
  bwScreenGraph::ScreenGraph& screen_graph;
  /** Reference to the screen-graph's context (convenience). */
  bwContext& context;

  std::optional<bwMouseButtonDragEvent> drag_event;

  bool isDragging();
  void changeContextHovered(bwScreenGraph::Node*, bwEvent&);
};

}  // namespace bWidgets
