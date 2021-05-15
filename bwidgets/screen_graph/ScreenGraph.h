#pragma once

#include "bwContext.h"
#include "bwEventDispatcher.h"

namespace bWidgets {
namespace bwScreenGraph {

class Node;
class LayoutNode;

class ScreenGraph {
  friend class Constructor;

 public:
  ScreenGraph() : event_dispatcher(*this)
  {
  }

  template<typename _NodeType>
  ScreenGraph(std::unique_ptr<_NodeType> _root_node)
      : event_dispatcher(*this), root_node(std::move(_root_node))
  {
  }

  void Root(std::unique_ptr<LayoutNode> _root_node)
  {
    root_node = std::move(_root_node);
  }

  auto Root() const -> LayoutNode&
  {
    return *root_node;
  }

  /** The context describing the state of this screen-graph */
  bwContext context;
  bwEventDispatcher event_dispatcher;

 private:
  std::unique_ptr<LayoutNode> root_node = nullptr;
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
