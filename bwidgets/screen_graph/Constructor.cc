#include <memory>

#include "Node.h"
#include "ScreenGraph.h"
#include "screen_graph/Iterators.h"

#include "Constructor.h"

namespace bWidgets {
namespace bwScreenGraph {

void Constructor::reconstruct(ScreenGraph& screen_graph, ConstructorFunc func)
{
  std::unique_ptr<Node> old_root = std::move(screen_graph.root_node);
  func();
  updateFromOld(screen_graph, *old_root, screen_graph.Root());
}

void Constructor::updateFromOld(ScreenGraph& new_screen_graph,
                                Node& old_subtree,
                                Node& new_subtree)
{
#if 0
  for (Node& iter_node : new_subtree) {
    bwWidget* widget = iter_node.Widget();
    if (!widget) {
      continue;
    }
  }
#endif
  //  screen_graph.event_dispatcher
}

}  // namespace bwScreenGraph
}  // namespace bWidgets
