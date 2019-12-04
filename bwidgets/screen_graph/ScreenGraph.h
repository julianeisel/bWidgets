#pragma once

#include "bwContext.h"
#include "bwEventDispatcher.h"
#include "bwPtr.h"

namespace bWidgets {
namespace bwScreenGraph {

class Node;
class LayoutNode;

class ScreenGraph {
 public:
  /** The context describing the state of this screen-graph */
  bwContext context;
  bwEventDispatcher event_dispatcher;

  template<typename _NodeType>
  ScreenGraph(bwPtr<_NodeType> _root_node)
      : event_dispatcher(*this), root_node(std::move(_root_node))
  {
  }

  LayoutNode& Root() const
  {
    return *root_node;
  }

 private:
  bwPtr<LayoutNode> root_node;
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
