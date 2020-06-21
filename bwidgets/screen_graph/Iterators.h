#pragma once

#include <iterator>

#include "Node.h"
#include "ScreenGraph.h"
#include "bwWidget.h"

namespace bWidgets {
namespace bwScreenGraph {

/**
 * \brief Iterator for pre-order (depth-first) traversal.
 *
 * Although recursion based algorithms are simple to understand (much simpler
 * than the internals of this iterator at least), iteration based algorithms
 * are often simpler to use, more readable (no need for lambdas or separate
 * functions) and can minimize stack usage. This iterator tries to give all of
 * these benefits.
 *
 * Stack memory should not be a concern with this iterator. It only stores a
 * few bytes for the current item. However it does allocate a bit of heap
 * memory to keep information about visited ancestors up to the root of
 * iteration. Even that only adds trivial overhead though.
 */
class PreOrderIterator {
 public:
  PreOrderIterator();
  PreOrderIterator(Node& node);
  ~PreOrderIterator();

  // Delete copy but keep move constructor for now. We store a parent path
  // below which would require deep copying. So prefer move over copy.
  PreOrderIterator(const PreOrderIterator&) = delete;
  PreOrderIterator(PreOrderIterator&&);

  auto operator!=(const PreOrderIterator&) const -> bool;
  auto operator*() -> Node&;
  auto operator++() -> PreOrderIterator&;

 private:
  void triggerIterationEnd();
  bool hasExceededLastSibling();

  union {
    Node* node;
    Node::ChildList::iterator node_iter;
  };

  Node* root = nullptr;
  bool is_root = true;
  // Ancestors up to (but excluding!) node that started iteration.
  std::list<Node::ChildList::iterator> ancestors;
};

/* PreOrderIterator is the default iterator (implicitly chosen when passing a
 * node as range-expression for range-based foor loops) */
auto begin(Node&) -> PreOrderIterator;
auto end(Node&) -> PreOrderIterator;
auto begin(ScreenGraph& screen_graph) -> PreOrderIterator;
auto end(ScreenGraph& screen_graph) -> PreOrderIterator;

}  // namespace bwScreenGraph
}  // namespace bWidgets
