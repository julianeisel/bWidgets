#include "Iterators.h"

using namespace bWidgets::bwScreenGraph;

PreOrderIterator::PreOrderIterator() : node(nullptr)
{
}

PreOrderIterator::PreOrderIterator(Node& node) : node(&node), root(&node)
{
}

bool PreOrderIterator::operator!=(const PreOrderIterator& other) const
{
  return (is_root != other.is_root) ||
         (is_root ? (node != other.node) : (node_iter != other.node_iter));
}

Node& PreOrderIterator::operator*()
{
  return is_root ? *node : **node_iter;
}

void PreOrderIterator::triggerIterationEnd()
{
  is_root = true;
  node = nullptr;
}

bool PreOrderIterator::hasExceededLastSibling()
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

PreOrderIterator& PreOrderIterator::operator++()
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
      }
    }
  }
  else {
    if (!is_root) {
      ancestors.push_back(node_iter);
    }
    node_iter = childs->begin();
    is_root = false;
  }

  return *this;
}

namespace bWidgets {
namespace bwScreenGraph {

PreOrderIterator begin(Node& node)
{
  return PreOrderIterator(node);
}

PreOrderIterator end(Node&)
{
  return {};
}

}  // namespace bwScreenGraph
}  // namespace bWidgets
