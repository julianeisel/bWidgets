#include "screen_graph/Node.h"

#include "PersistentPtr.h"

namespace bWidgets::bwScreenGraph {

auto PersistentNodeRegistry::make_persistent_ptr(Node* node, const char* debug_name)
    -> PersistentNodePtr
{
  if (!node) {
    /* Empty pointer. Don't add to registry. */
    return {};
  }

  if (PersistentNodePtr* ptr = find_matching_from_address(*node)) {
    /* Copy from existing pointer (increases user count) */
    return PersistentNodePtr{*ptr};
  }
  assert(!hasPointerWithAddress(*node));
  /* If this happened, the widget is either ambiguous (we can't uniquely identify it) or is still
   * dangling from the last redraw. */
  assert(!find_matching(*node));

  PersistentNodePtr new_ptr{*node, debug_name};
  registry_.push_back(new_ptr);
  return new_ptr;
}

/**
 * Try to find a registered pointer to \a needle using value equality checking.
 */
auto PersistentNodeRegistry::find_matching(const Node& needle) -> PersistentNodePtr*
{
  for (PersistentNodePtr& ptr : registry_) {
    /* Ignore unset ptr. */
    if (!ptr) {
      continue;
    }
    /* Same address. */
    if (&*ptr == &needle) {
      return &ptr;
    }

    if (*ptr == needle) {
      return &ptr;
    }
  }

  return nullptr;
}

/**
 * Use memory address to find a registered pointer for \a node.
 */
auto PersistentNodeRegistry::find_matching_from_address(const Node& node) -> PersistentNodePtr*
{
  iterator ptr_iterator = find_matching_from_address_iter(node);
  if (ptr_iterator != registry_.end()) {
    return &*ptr_iterator;
  }
  return nullptr;
}

/**
 * Use memory address to find a registered pointer for \a node (variation of
 * #PersistentNodeRegistry::find_matching_from_address that returns a pointer).
 */
auto PersistentNodeRegistry::find_matching_from_address_iter(const Node& node) -> iterator
{
  return std::find_if(registry_.begin(), registry_.end(), [&node](const PersistentNodePtr& ptr) {
    return ptr.get() == &node;
  });
}

void PersistentNodeRegistry::updateMatching(Node& node)
{
  if (PersistentNodePtr* ptr = find_matching(node)) {
    ptr->rebind_to_node(&node);
  }
}

void PersistentNodeRegistry::removePtrsToNode(const Node& node)
{
  iterator ptr_iter = find_matching_from_address_iter(node);
  if (ptr_iter != registry_.end()) {
    PersistentNodePtr& ptr = *ptr_iter;
    ptr.rebind_to_node(nullptr);
    registry_.erase(ptr_iter);
  }
  /* There shouldn't be another ptr for this node. */
  assert(!hasPointerWithAddress(node));
}

/**
 * Check registry for pointers that are not actually used anywhere (determined through
 * ref-counting of the nested `std::shared_ptr`), and unregister them, causing the pointers to be
 * destructed.
 */
void PersistentNodeRegistry::cleanupUnused()
{
  /* Remove all pointers that are only used by the registry and have no other users. The shared
   * pointer will be implicitly destructed then. */

  /* Erase-Remove idiom. */
  registry_.erase(std::remove_if(registry_.begin(),
                                 registry_.end(),
                                 [](const PersistentNodePtr& ptr) {
                                   assert(ptr.node_ptr_.use_count() >= 1);
                                   return !ptr || ptr.node_ptr_.unique();
                                 }),
                  registry_.end());
}

/**
 * Check if \a node is in the registry, using the address __not__ by doing value comparison.
 */
auto PersistentNodeRegistry::hasPointerWithAddress(const Node& node) -> bool
{
  return find_matching_from_address_iter(node) != registry_.end();
}

}  // namespace bWidgets::bwScreenGraph
