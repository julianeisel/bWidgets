#include "screen_graph/Node.h"

#include "PersistentRef.h"

namespace bWidgets::bwScreenGraph {

auto PersistentNodeRegistry::make_persistent_ref(Node* node, const char* debug_name)
    -> PersistentNodeRef
{
  if (!node) {
    /* Empty reference. Don't add to registry. */
    return {};
  }

  if (PersistentNodeRef* ref = find_exact_matching_ref(*node)) {
    /* Copy from existing reference (increases user count) */
    return PersistentNodeRef{*ref};
  }
  assert(!hasExactRef(*node));
  /* If this happened, the widget is either ambiguous (we can't uniquely identify it) or is still
   * dangling from the last redraw. */
  assert(!find_matching_ref(*node));

  PersistentNodeRef new_ref{*node, debug_name};
  registry_.push_back(new_ref);
  return new_ref;
}

/**
 * Try to find a reference to \a needle using value equality checking.
 */
auto PersistentNodeRegistry::find_matching_ref(const Node& needle) -> PersistentNodeRef*
{
  for (PersistentNodeRef& ref : registry_) {
    /* Ignore unset references. */
    if (!ref) {
      continue;
    }
    /* Same address. */
    if (&*ref == &needle) {
      return &ref;
    }

    if (*ref == needle) {
      return &ref;
    }
  }

  return nullptr;
}

/**
 * Use memory address to find a reference for \a node.
 */
auto PersistentNodeRegistry::find_exact_matching_ref(const Node& node) -> PersistentNodeRef*
{
  iterator ref_iter = find_exact_matching_ref_iter(node);
  if (ref_iter != registry_.end()) {
    return &*ref_iter;
  }
  return nullptr;
}

/**
 * Use memory address to find a reference for \a node (variation of
 * #PersistentNodeRegistry::find_exact_matching_ref that returns a pointer).
 */
auto PersistentNodeRegistry::find_exact_matching_ref_iter(const Node& node) -> iterator
{
  return std::find_if(registry_.begin(), registry_.end(), [&node](const PersistentNodeRef& ref) {
    return ref.get() == &node;
  });
}

void PersistentNodeRegistry::updateMatchingRefs(Node& node)
{
  if (PersistentNodeRef* ref = find_matching_ref(node)) {
    ref->rebind_referenced_node(&node);
  }
}

void PersistentNodeRegistry::clearRefsToNode(const Node& node)
{
  iterator ref_iter = find_exact_matching_ref_iter(node);
  if (ref_iter != registry_.end()) {
    PersistentNodeRef& ref = *ref_iter;
    ref.rebind_referenced_node(nullptr);
    registry_.erase(ref_iter);
  }
  /* There shouldn't be another ref for this node. */
  assert(!hasExactRef(node));
}

/**
 * Check registry for references that are not actually used anywhere (determined through
 * ref-counting of the nested `std::shared_ptr`), and unregister them, causing the references to be
 * destructed.
 */
void PersistentNodeRegistry::cleanupUnusedRefs()
{
  /* Remove all references that are only used by the registry and have no other users. The shared
   * pointer will be implicitly destructed then. */

  /* Erase-Remove idiom. */
  registry_.erase(std::remove_if(registry_.begin(),
                                 registry_.end(),
                                 [](const PersistentNodeRef& ref) {
                                   assert(ref.node_ref_.use_count() >= 1);
                                   return !ref || ref.node_ref_.unique();
                                 }),
                  registry_.end());
}

/**
 * Check if \a node is in the registry, using the address __not__ by doing value comparison.
 */
auto PersistentNodeRegistry::hasExactRef(const Node& node) -> bool
{
  return find_exact_matching_ref_iter(node) != registry_.end();
}

}  // namespace bWidgets::bwScreenGraph
