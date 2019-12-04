#pragma once

#include <list>

#include "bwLayoutInterface.h"
#include "bwPtr.h"
#include "bwWidget.h"

namespace bWidgets {
namespace bwScreenGraph {

/**
 * \brief The base data-structure for a screen-graph node
 *
 * Screen-graph nodes are the atomical components of a screen-graph, which is
 * key for the bWidgets design. If you're not familiar with our screen-graph
 * concept, you should really have a look at the
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
  friend class PreOrderIterator;

 public:
  using ChildList = std::list<bwPtr<Node>>;
  using ChildIterator = ChildList::iterator;

  Node() = default;
  virtual ~Node() = default;

  virtual const ChildList *Children() const
  {
    return nullptr;
  }
  virtual ChildList *Children()
  {
    return nullptr;
  }

  const Node *Parent() const
  {
    return parent;
  }

  virtual bwLayoutInterface *Layout() const
  {
    return nullptr;
  }

  virtual bwWidget *Widget() const
  {
    return nullptr;
  }

 private:
  Node *parent;
};

/**
 * \brief Node for aligning children to a specific layout.
 */
class LayoutNode : virtual public Node {
  friend class Builder;
  friend class PreOrderIterator;

 public:
  const ChildList *Children() const override
  {
    return &children;
  }
  ChildList *Children() override
  {
    return &children;
  }

  bwLayoutInterface *Layout() const override
  {
    return &*layout;
  }

 private:
  ChildList children;
  bwPtr<bwLayoutInterface> layout;
};

/**
 * \brief Node representing a single widget with no children.
 */
class WidgetNode : virtual public Node {
  friend class Builder;
  friend class PreOrderIterator;

 public:
  bwWidget *Widget() const override
  {
    return &*widget;
  }

 private:
  bwPtr<bwWidget> widget;
};

/**
 * \brief Node representing a widget with children.
 *
 * Note virtual inheritance of LayoutNode and WidgetNode, required to solve
 * diamond problems.
 */
class ContainerNode : public LayoutNode, public WidgetNode {
  friend class Builder;

 public:
  bwWidget *Widget() const override
  {
    return &*widget;
  }

 private:
  bwPtr<bwWidget> widget;
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
