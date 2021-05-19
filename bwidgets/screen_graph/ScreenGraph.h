#pragma once

#include <vector>

#include "bwContext.h"
#include "bwEventDispatcher.h"

#include "PersistentPtr.h"

namespace bWidgets {
namespace bwScreenGraph {

class Node;
class LayoutNode;

class ScreenGraph {
  friend class Constructor;

 public:
  using RootNodeType = std::unique_ptr<LayoutNode>;

  ScreenGraph() : event_dispatcher(*this)
  {
  }

  template<typename _NodeType>
  ScreenGraph(std::unique_ptr<_NodeType> _root_node)
      : event_dispatcher(*this), root_node(std::move(_root_node))
  {
  }

  inline void Root(RootNodeType _root_node)
  {
    root_node = std::move(_root_node);
  }

  inline auto Root() const -> LayoutNode*
  {
    return root_node.get();
  }

  friend inline PersistentNodePtr make_persistent_ptr(ScreenGraph& screen_graph,
                                                      Node* node,
                                                      const char* debug_name)
  {
    return screen_graph.persistent_node_registry_.make_persistent_ptr(node, debug_name);
  }

  /** The context describing the state of this screen-graph */
  bwContext context;
  bwEventDispatcher event_dispatcher;

 private:
  RootNodeType root_node = nullptr;

  PersistentNodeRegistry persistent_node_registry_;
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
