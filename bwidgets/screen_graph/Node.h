#pragma once

#include <iostream>
#include <list>
#include <memory>
#include <optional>

#include "bwRectangle.h"

namespace bWidgets {

class bwContainerWidget;
class bwLayoutInterface;
class bwWidget;

namespace bwScreenGraph {

class EventHandler;

/**
 * \brief The base data-structure for a screen-graph node
 *
 * Screen-graph nodes are the components of a screen-graph, which is key for the bWidgets design.
 * If you're not familiar with our screen-graph concept, you should really have a look at the
 * [bWidgets design overview](md_docs_bWidgets_design_overview.html).
 *
 * There are three kinds of screen-graph elements/nodes:
 * * Widget nodes (node representing a simple widget)
 * * Layout nodes (node with children aligned in specified layout)
 * * Container nodes (combination of both, widget with children aligned in
 *   layout - i.e. panel)
 *
 * To ensure separation of concerns, the screen-graph node must not have any
 * functionality of its own (no non-trivial member functions). Any functionality
 * must be implemented using friend classes and functions. For example to build
 * a screen-graph and the necessary nodes, a screen-graph builder class should
 * be used and be declared as friend of the `Node` class.
 * Having to declare those helpers as friends may turn out to an annoyance
 * with small benefits. In that case we should just make data public.
 */
class Node {
  friend class Builder;

 public:
  using ChildList = std::list<std::unique_ptr<Node>>;
  using ChildIterator = ChildList::iterator;

  Node() = default;
  virtual ~Node();

  virtual auto Children() const -> const ChildList*;
  virtual auto Children() -> ChildList*;
  virtual auto childrenVisible() const -> bool;
  virtual auto Layout() const -> bwLayoutInterface*;
  virtual auto Widget() const -> bwWidget*;

  auto Parent() const -> Node*;
  auto eventHandler() const -> EventHandler*;

  /**
   * Check if the type of this node matches another one. Widget and layout nodes also compare
   * the type of their contained widget/layout.
   */
  virtual auto matchesType(const Node&) const -> bool;
  /**
   * Check if the node's data matches another one's. Widget and layout nodes also compare the data
   * of their contained widget/layout.
   */
  virtual auto matches(const Node&) const -> bool = 0;
  /**
   * Move the state of one node to another, if the node types match. Otherwise doesn't do
   * anything.
   */
  virtual void moveState(Node& from);

  virtual auto Rectangle() const -> bwRectanglePixel = 0;
  virtual auto MaskRectangle() const -> std::optional<bwRectanglePixel> = 0;
  virtual auto isVisible() const -> bool = 0;

 private:
  Node* parent{nullptr};
  std::unique_ptr<EventHandler> handler{nullptr};
};

/**
 * \brief Node for aligning children to a specific layout.
 */
class LayoutNode : virtual public Node {
  friend class Builder;

 public:
  ~LayoutNode();

  auto Children() const -> const ChildList* override;
  auto Children() -> ChildList* override;

  auto Layout() const -> bwLayoutInterface* override;
  auto Rectangle() const -> bwRectanglePixel override;
  auto MaskRectangle() const -> std::optional<bwRectanglePixel> override;
  auto isVisible() const -> bool override;

  auto matchesType(const Node&) const -> bool override;
  auto matches(const Node&) const -> bool override;

 private:
  ChildList children;
  std::unique_ptr<bwLayoutInterface> layout;
};

/**
 * \brief Node representing a single widget with no children.
 */
class WidgetNode : virtual public Node {
  friend class Builder;

 public:
  auto Widget() const -> bwWidget* override;
  auto Rectangle() const -> bwRectanglePixel override;
  auto MaskRectangle() const -> std::optional<bwRectanglePixel> override;
  auto isVisible() const -> bool override;

  auto matchesType(const Node&) const -> bool override;
  auto matches(const Node&) const -> bool override;
  void moveState(Node&) override;

 private:
  std::unique_ptr<bwWidget> widget;
};

/**
 * \brief Node representing a widget with children.
 *
 * Note virtual inheritance of LayoutNode and WidgetNode, required to solve
 * diamond problems.
 */
class ContainerNode : public LayoutNode, public WidgetNode {
 public:
  auto Children() const -> const ChildList* override;
  auto Children() -> ChildList* override;
  auto Layout() const -> bwLayoutInterface* override;
  auto Widget() const -> bwWidget* override;
  auto ContainerWidget() const -> bwContainerWidget&;

  auto Rectangle() const -> bwRectanglePixel override;
  auto ContentRectangle() const -> bwRectanglePixel;
  auto MaskRectangle() const -> std::optional<bwRectanglePixel> override;

  auto isVisible() const -> bool override;
  auto childrenVisible() const -> bool override;

  auto matchesType(const Node&) const -> bool override;
  auto matches(const Node&) const -> bool override;
  void moveState(Node&) override;
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
