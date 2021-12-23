#include <iostream>
#include <memory>

#include "Node.h"
#include "ScreenGraph.h"
#include "bwWidget.h"
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
   * function, so persistent node pointers can get updated/rebound. */
  std::unique_ptr<Node> old_root = std::move(screen_graph.root_node);

  addAlwaysPersistentPointers(screen_graph, *old_root);

  screen_graph.Root(construct_func());
  if (!screen_graph.Root()) {
    return;
  }

  PersistentNodeRegistry::UpdateFn update_fn = [](Node& old, Node& new_) { new_.moveState(old); };
  updatePersistentPointersFromOld(screen_graph, *screen_graph.Root(), *old_root, update_fn);
}

/**
 * Some widget types are always persistent by design. This is simply established by adding them
 */
void Constructor::addAlwaysPersistentPointers(ScreenGraph& screen_graph, const Node& subtree)
{
  for (Node& iter_node : const_cast<Node&>(subtree)) {
    if (bwWidget* widget = iter_node.Widget()) {
      if (widget->getState<bwWidgetState>().alwaysPersistent()) {
        const std::string* label = widget->getLabel();
        make_persistent_ptr(
            screen_graph, &iter_node, label ? label->c_str() : "Some always persistent pointer");
      }
    }
  }
}

void Constructor::updatePersistentPointersFromOld(ScreenGraph& screen_graph,
                                                  Node& new_subtree,
                                                  const Node& old_subtree,
                                                  PersistentNodeRegistry::UpdateFn update_fn)
{
  for (Node& iter_node : new_subtree) {
    /* There is a matching persistent pointer to `iter_node`. That means we found a reconstructed
     * node that other code wants to keep a persistent pointer to. Update it. */
    screen_graph.persistent_node_registry_.updateMatching(iter_node, update_fn);
  }
  clearDanglingPersistentPointers(screen_graph, old_subtree);

  screen_graph.persistent_node_registry_.cleanupUnused();

  /* Debug-only sanity check: Ensure the registry doesn't point to any old nodes anymore. They
   * will be freed when leaving this current function. */
#ifndef NDEBUG
  checkSanePersistentPointers(screen_graph, old_subtree);
#endif
}

/**
 * There may be persistent pointers that could not be found in the new tree. Either because they
 * were removed or their equality check failed (the #bwWidget::operator==() override). Such
 * pointers must be unset.
 */
void Constructor::clearDanglingPersistentPointers(ScreenGraph& screen_graph,
                                                  const Node& old_subtree)
{
  for (const Node& old_node : const_cast<Node&>(old_subtree)) {
    screen_graph.persistent_node_registry_.removePtrsToNode(old_node);
  }
}

void Constructor::checkSanePersistentPointers(ScreenGraph& screen_graph, const Node& old_subtree)
{
  for (const Node& old_node : const_cast<Node&>(old_subtree)) {
    if (screen_graph.persistent_node_registry_.hasPointerWithAddress(old_node)) {
      std::cerr << "Fatal error: Persistent node registry points to data to be destructed. This "
                   "is a bug!"
                << std::endl;
      assert(0);
    }
  }
}

}  // namespace bWidgets::bwScreenGraph
