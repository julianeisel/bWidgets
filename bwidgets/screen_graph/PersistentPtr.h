#pragma once

#include <functional>
#include <list>
#include <memory>

/**
 * \file
 *
 * Node pointers that stay valid over reconstructions of the screen-graph.
 * Each PersistentNodePtr object and all copies created from it are automatically updated to a
 * matching new widget as part of the reconstruction (`Constructor::reconstruct()`).
 *
 * `PersistentNodePtr` objects are assignable pointers supporting copy and move semantics.
 * Reference counting is used internally to make sure the pointer is kept alive and keeps being
 * updated until it's not used any more.<br/>
 * The only way to create or reassign a `PersistentNodePtr` for a given node is through
 * `ScreenGraph::make_persistent_ptr()`.
 *
 *
 * ### Usage
 *
 * * Define a persistent pointer variable:
 *   ```cc
 *   PersistentNodePtr node_ptr;
 *   ```
 * * Create the actual pointer from a given node:
 *   ```cc
 *   node_ptr = make_persistent_ptr(screen_graph, node, "My node ptr");
 *   ```
 *   The last parameter is an identifier for the pointer which is only used for debugging.
 *
 *
 * ### Notes
 *
 * Note that the correct behavior of the updating relies on widgets being identifiable. Mainly,
 * this means a good `bwWidget::matches()` implementation. If this isn't given, the pointer
 * will be invalidated. Hence, they should be checked for validity before access:
 * ```cc
 * if (node_ptr) {
 *   do_something(node_ptr);
 * }
 * ```
 *
 * ### Implementation
 *
 * Internally this uses a `std::shared_ptr<Node *>` which a) manages storage/access so we can
 * point to the same node from all instances of a `PersistentNodePtr` and b) does the reference
 * counting needed to know when to stop updating and destruct the pointer.
 * The updating is made possible by keeping a register of all persistent pointers in the
 * screen-graph (via `PersistentNodeRegistry`).
 */

namespace bWidgets {
class bwWidget;
}

namespace bWidgets::bwScreenGraph {
class Node;

class PersistentNodePtr {
  /* Note that even though this is non-null, the wrapped `Node *` may still be null. */
  std::shared_ptr<Node*> node_ptr_ = nullptr;
#ifndef NDEBUG
  /** For debugging, a name for this pointer, set with
   * #PersistentNodeRegistry::make_persistent_ptr(). */
  const char* name_ = nullptr;
#endif

 public:
  constexpr PersistentNodePtr() = default;
  /**
   * Allow construction from `nullptr`. Kinda redundant since that's what the default constructor
   * does too, but an explicit `PersistentNodePtr ptr = nullptr` makes the default state clear.
   */
  constexpr PersistentNodePtr(std::nullptr_t) : PersistentNodePtr()
  {
  }
  /* It's critical that copy and move construction/assignment call the matching
   * construction/assignment of `node_ptr_`, so that the shared pointer logic works. The default
   * copy and move constructors/assignment-operators do memberwise copy/move, so they do exactly
   * what's needed. */
  PersistentNodePtr(const PersistentNodePtr&) = default;
  PersistentNodePtr(PersistentNodePtr&&) = default;
  PersistentNodePtr& operator=(const PersistentNodePtr& other) = default;
  PersistentNodePtr& operator=(PersistentNodePtr&& other) = default;

  inline auto operator*() const -> const Node&
  {
    return **node_ptr_;
  }
  inline auto operator*() -> Node&
  {
    return **node_ptr_;
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
    return bool(*this) ? *node_ptr_ : nullptr;
  }
  inline auto get() -> Node*
  {
    return bool(*this) ? *node_ptr_ : nullptr;
  }

  explicit inline operator bool() const
  {
    /* Pass check on to the shared pointer. */
    return bool(node_ptr_) && bool(*node_ptr_);
  }

 private:
  /* Registers and manages (updates) the pointers. */
  friend class PersistentNodeRegistry;

  /**
   * Create a new persistent node pointer. Private constructor, it must be called through
   * #PersistentNodeRegistry::make_persistent_ptr().
   */
  explicit PersistentNodePtr(Node& node, const char* debug_name)
      : node_ptr_(std::make_shared<Node*>(&node))
#ifndef NDEBUG
        ,
        name_(debug_name)
#endif
  {
    (void)debug_name;
  }

  /**
   * Update all instances of this `PersistentNodePtr` to point to \a new_node.
   */
  inline void rebind_to_node(Node* new_node)
  {
    *node_ptr_ = new_node;
  }
};

class PersistentNodeRegistry {
  /* TODO a list for now. Should become a hash table, for efficient lookups. */
  std::list<PersistentNodePtr> registry_;
  using iterator = decltype(registry_)::iterator;

 public:
  using UpdateFn = std::function<void(Node& old_, Node& new_)>;

  auto make_persistent_ptr(Node* node, const char* debug_name) -> PersistentNodePtr;

  void updateMatching(Node& node, std::optional<UpdateFn> update_fn = nullptr);
  void removePtrsToNode(const Node& node);
  void cleanupUnused();
  auto hasPointerWithAddress(const Node& node) -> bool;

 private:
  auto find_matching(const Node& needle) -> PersistentNodePtr*;
  auto find_matching_from_address(const Node& node) -> PersistentNodePtr*;
  auto find_matching_from_address_iter(const Node& node) -> iterator;
};

}  // namespace bWidgets::bwScreenGraph
