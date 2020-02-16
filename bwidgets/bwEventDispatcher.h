#pragma once

#include "bwEvent.h"
#include "bwOptional.h"
#include "bwPoint.h"

namespace bWidgets {

class bwContext;
namespace bwScreenGraph {
class ScreenGraph;
class Node;
class EventHandler;
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
  /* Event to bubble is not passed here, lambdas can capture it while keeping exact type (no cast
   * needed). */
  using EventBubbleFunctor =
      std::function<void(const bwScreenGraph::Node&, bwScreenGraph::EventHandler& handler)>;

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

  bwOptional<bwMouseButtonDragEvent> drag_event;

  bool isDragging();
  void changeContextHovered(bwScreenGraph::Node*, bwEvent&);
};

}  // namespace bWidgets
