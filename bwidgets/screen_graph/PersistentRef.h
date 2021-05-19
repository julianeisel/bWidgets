#pragma once

#include <functional>
#include <list>
#include <memory>

/**
 * \file
 *
 * Node references that stay valid over reconstructions of the screen-graph.
 * Each PersistentNodeRef object and all copies created from it are automatically updated to a
 * matching new widget as part of the reconstruction (`Constructor::reconstruct()`).
 *
 * `PersistentNodeRef` objects are assignable references
 * supporting copy and move semantics. Reference counting is used internally to make sure the
 * reference is kept alive and keeps being updated until it's not used any more.<br/>
 * The only way to create or reassign a `PersistentNodeReference` for a given node is through
 * `ScreenGraph::make_persistent_ref()`.
 *
 *
 * ### Usage
 *
 * * Define a reference variable:
 *   ```cc
 *   PersistentNodeRef node_ref;
 *   ```
 * * Create the actual reference from a given node:
 *   ```cc
 *   node_ref = make_persistent_ref(screen_graph, node, "My node ref");
 *   ```
 *   The last parameter is an identifier for the reference which is only used for debugging.
 *
 *
 * ### Notes
 *
 * Note that the correct behavior of the updating relies on widgets being identifiable. Mainly,
 * this means a good `bwWidget::operator==()` implementation. If this isn't given, the reference
 * will be invalidated. Hence, they should be checked for validity before access:
 * ```cc
 * if (node_ref) {
 *   do_something(node_ref);
 * }
 * ```
 *
 * ### Implementation
 *
 * Internally this uses a `std::shared_ptr<Node *>` which a) manages storage/access so we can
 * reference the same node from all instances of a `PersistentNodeRef` and b) does the reference
 * counting needed to know when to stop updating and destruct the reference.
 * The updating is made possible by keeping a register of all persistent references in the
 * screen-graph (via `PersistentNodeRegistry`).
 */

namespace bWidgets {
class bwWidget;
}

namespace bWidgets::bwScreenGraph {
class Node;

class PersistentNodeRef {
  using NodeRef = Node*;
  /* Note that even though this is non-null, the wrapped `Node *` may still be null. */
  std::shared_ptr<NodeRef> node_ref_ = nullptr;
#ifndef NDEBUG
  /** For debugging, a name for this reference, set with
   * #PersistentNodeRegistry::make_persistent_ref(). */
  const char* name_ = nullptr;
#endif

 public:
  constexpr PersistentNodeRef() = default;
  /**
   * Allow construction from `nullptr`. Kinda redundant since that's what the default constructor
   * does too, but an explicit `PersistentNodeRef ref = nullptr` makes the default state clear.
   */
  constexpr PersistentNodeRef(nullptr_t) : PersistentNodeRef()
  {
  }
  /* It's critical that copy and move construction/assignment call the matching
   * construction/assignment of `node_ref_`, so that the shared pointer logic works. The default
   * copy and move constructors/assignment-operators do memberwise copy/move, so they do exactly
   * what's needed. */
  PersistentNodeRef(const PersistentNodeRef&) = default;
  PersistentNodeRef(PersistentNodeRef&&) = default;
  PersistentNodeRef& operator=(const PersistentNodeRef& other) = default;
  PersistentNodeRef& operator=(PersistentNodeRef&& other) = default;

  inline auto operator*() const -> const Node&
  {
    return **node_ref_;
  }
  inline auto operator*() -> Node&
  {
    return **node_ref_;
  }

  inline auto operator->() const -> const Node*
  {
    return get();
  }
  inline auto operator->() -> Node*
  {
    return get();
  }

  inline auto get() const -> const Node*
  {
    return bool(*this) ? *node_ref_ : nullptr;
  }
  inline auto get() -> Node*
  {
    return bool(*this) ? *node_ref_ : nullptr;
  }

  explicit inline operator bool() const
  {
    /* Pass check on to the shared pointer. */
    return bool(node_ref_) && bool(*node_ref_);
  }

 private:
  /* Registers and manages (updates) the references. */
  friend class PersistentNodeRegistry;

  /**
   * Create a new persistent node reference. Private constructor, it must be called through
   * #PersistentNodeRegistry::make_persistent_ref().
   */
  explicit PersistentNodeRef(Node& node, const char* debug_name)
      : node_ref_(std::make_shared<NodeRef>(&node))
#ifndef NDEBUG
        ,
        name_(debug_name)
#endif
  {
    (void)debug_name;
  }

  /**
   * Update all instances of this `PersistentNodeRef` to point to \a new_node.
   */
  inline void rebind_referenced_node(Node* new_node)
  {
    *node_ref_ = new_node;
  }
};

class PersistentNodeRegistry {
  /* TODO a list for now. Should become a hash table, for efficient lookups. */
  std::list<PersistentNodeRef> registry_;
  using iterator = decltype(registry_)::iterator;

 public:
  auto make_persistent_ref(Node* node, const char* debug_name) -> PersistentNodeRef;

  void updateMatchingRefs(Node& node);
  void clearRefsToNode(const Node& node);
  void cleanupUnusedRefs();
  auto hasExactRef(const Node& node) -> bool;

 private:
  auto find_matching_ref(const Node& needle) -> PersistentNodeRef*;
  auto find_exact_matching_ref(const Node& node) -> PersistentNodeRef*;
  auto find_exact_matching_ref_iter(const Node& node) -> iterator;
};

}  // namespace bWidgets::bwScreenGraph
