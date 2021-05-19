#include <iostream>
#include <memory>

#include "Node.h"
#include "ScreenGraph.h"
#include "screen_graph/Iterators.h"

#include "Constructor.h"

namespace bWidgets::bwScreenGraph {

/**
 * (Re-)construct the screen graph via the callback \a construct_func. This must return the new
 * root node to be used.
 */
void Constructor::reconstruct(ScreenGraph& screen_graph, ConstructionFunc construct_func)
{
  /* Take over ownership of the old screen graph tree. Keep it alive until the end of this
   * function, so persistent node references can get updated/rebound. */
  std::unique_ptr<Node> old_root = std::move(screen_graph.root_node);

  screen_graph.Root(construct_func());
  if (!screen_graph.Root()) {
    return;
  }

  updatePersistentRefsFromOld(screen_graph, *screen_graph.Root(), *old_root);

  /* Debug-only sanity check: Ensure the registry doesn't reference any old nodes anymore. They
   * will be freed when leaving this current function. */
#ifndef NDEBUG
  checkSanePersistentRefs(screen_graph, *old_root);
#endif
}

void Constructor::updatePersistentRefsFromOld(ScreenGraph& screen_graph,
                                              Node& new_subtree,
                                              const Node& old_subtree)
{
  for (Node& iter_node : new_subtree) {
    /* There is a matching reference to `iter_node`. That means we found a reconstructed node
     * that other code wants to keep a persistent reference to. Update it. */
    screen_graph.persistent_node_registry_.updateMatchingRefs(iter_node);
  }
  clearDanglingPersistentRefs(screen_graph, old_subtree);

  screen_graph.persistent_node_registry_.cleanupUnusedRefs();
}

/**
 * There may be persistent references that could not be found in the new tree. Either because they
 * were removed or their equality check failed (the #bwWidget::operator==() override). Such
 * references must be cleared.
 */
void Constructor::clearDanglingPersistentRefs(ScreenGraph& screen_graph, const Node& old_subtree)
{
  for (const Node& old_node : const_cast<Node&>(old_subtree)) {
    screen_graph.persistent_node_registry_.clearRefsToNode(old_node);
  }
}

void Constructor::checkSanePersistentRefs(ScreenGraph& screen_graph, const Node& old_subtree)
{
  for (const Node& old_node : const_cast<Node&>(old_subtree)) {
    if (screen_graph.persistent_node_registry_.hasExactRef(old_node)) {
      std::cerr << "Fatal error: Persistent node registry references data to be destructed. "
                   "This is a bug!"
                << std::endl;
      assert(0);
    }
  }
}

}  // namespace bWidgets::bwScreenGraph
