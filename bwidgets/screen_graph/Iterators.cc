#include "Iterators.h"
#include <cassert>

namespace bWidgets {
namespace bwScreenGraph {

PreOrderIterator::PreOrderIterator() : node(nullptr)
{
}

PreOrderIterator::~PreOrderIterator()
{
}

PreOrderIterator::PreOrderIterator(Node& node) : node(&node), root(&node)
{
}

PreOrderIterator::PreOrderIterator(PreOrderIterator&& other)
    : ancestors(std::move(other.ancestors))
{
  if (other.is_root) {
    is_root = true;
    node = other.node;
    other.node = nullptr;
  }
  else {
    is_root = false;
    node_iter = other.node_iter;
    other.node_iter = {};
  }
  root = other.root;
  other.root = nullptr;
}

auto PreOrderIterator::operator!=(const PreOrderIterator& other) const -> bool
{
  return (is_root != other.is_root) ||
         (is_root ? (node != other.node) : (node_iter != other.node_iter));
}

auto PreOrderIterator::operator*() -> Node&
{
  return is_root ? *node : **node_iter;
}

void PreOrderIterator::triggerIterationEnd()
{
  is_root = true;
  node = nullptr;
}

auto PreOrderIterator::hasExceededLastSibling() -> bool
{
  assert(is_root == false);
  // Check if node_iter points at the parents children.end()
  if (ancestors.empty()) {
    return !root->Children() || (node_iter == root->Children()->end());
  }
  else {
    return !(*ancestors.back())->Children() ||
           (node_iter == (*ancestors.back())->Children()->end());
  }
}

auto PreOrderIterator::operator++() -> PreOrderIterator&
{
  Node::ChildList* childs = is_root ? node->Children() : (*node_iter)->Children();

  if (!childs || childs->empty()) {
    if (is_root) {
      triggerIterationEnd();
      return *this;
    }

    node_iter++;
    if (hasExceededLastSibling()) {
      if (ancestors.empty()) {
        triggerIterationEnd();
        return *this;
      }
      /* Go up the hierarchy until we find a parent with siblings that
       * can be visited. Once hasExceededLastSibling returns false, we
       * have a valid item that can be returned. */
      for (auto parent_iter = ancestors.rbegin(); hasExceededLastSibling(); ++parent_iter) {
        if (ancestors.empty() || parent_iter == ancestors.rend()) {
          triggerIterationEnd();
          break;
        }

        // next candidate for the iterator is the parents next sibling.
        node_iter = ++(*parent_iter);
        ancestors.pop_back();
        if (ancestors.empty()) {
          break;
        }
      }
    }
  }
  else {
    if (!is_root) {
      ancestors.push_back(node_iter);
    }
    node = nullptr;
    node_iter = childs->begin();
    is_root = false;
  }

  return *this;
}

auto begin(Node& node) -> PreOrderIterator
{
  return PreOrderIterator(node);
}
auto end(Node&) -> PreOrderIterator
{
  return {};
}
auto begin(ScreenGraph& screen_graph) -> PreOrderIterator
{
  if (!screen_graph.Root()) {
    return {};
  }
  return begin(*screen_graph.Root());
}
auto end(ScreenGraph& screen_graph) -> PreOrderIterator
{
  if (!screen_graph.Root()) {
    return {};
  }
  return end(*screen_graph.Root());
}

}  // namespace bwScreenGraph
}  // namespace bWidgets
