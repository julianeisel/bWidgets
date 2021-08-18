#pragma once

namespace bWidgets {
namespace bwScreenGraph {
class Node;
class LayoutNode;

class ScreenGraph;

/**
 * The constructor represents the _Construction_ stage of the screen-graph pipeline. It is the
 * entry point to all further screen-graph building and is responsible for keeping widget state
 * updated over redraws. Since the screen-graph is reconstructed from scratch for each redraw,
 * these two tasks belong closely together.
 */
class Constructor {
 public:
  using ConstructionFunc = std::function<std::unique_ptr<LayoutNode>()>;

  static void reconstruct(ScreenGraph& screen_graph, ConstructionFunc construct_func);

 private:
  static void addAlwaysPersistentPointers(ScreenGraph& screen_graph, const Node& old_subtree);
  static void updatePersistentPointersFromOld(
      ScreenGraph& screen_graph,
      Node& new_subtree,
      const Node& old_subtree,
      bWidgets::bwScreenGraph::PersistentNodeRegistry::UpdateFn update_fn);
  static void clearDanglingPersistentPointers(ScreenGraph& screen_graph, const Node& old_subtree);
  static void checkSanePersistentPointers(bWidgets::bwScreenGraph::ScreenGraph& screen_graph,
                                          const Node& old_subtree);
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
