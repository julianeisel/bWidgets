#pragma once

namespace bWidgets {
namespace bwScreenGraph {
class Node;
class LayoutNode;

class ScreenGraph;

class Constructor {
 public:
  using ConstructionFunc = std::function<std::unique_ptr<LayoutNode>()>;

  static void reconstruct(ScreenGraph& screen_graph, ConstructionFunc construct_func);

 private:
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
