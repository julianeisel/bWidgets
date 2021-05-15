#pragma once

namespace bWidgets {
namespace bwScreenGraph {

class ScreenGraph;

class Constructor {
 public:
  using ConstructorFunc = std::function<void()>;

  static void reconstruct(ScreenGraph& screen_graph, ConstructorFunc func);

 public:
  static void updateFromOld(ScreenGraph& screen_graph, Node& old_subtree, Node& new_subtree);
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
