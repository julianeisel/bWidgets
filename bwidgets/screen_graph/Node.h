#pragma once

#include <list>

#include "bwContainerWidget.h"
#include "bwLayoutInterface.h"
#include "bwWidget.h"

namespace bWidgets {
namespace bwScreenGraph {

class EventHandler;

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

 public:
  using ChildList = std::list<std::unique_ptr<Node>>;
  using ChildIterator = ChildList::iterator;

  Node() = default;
  virtual ~Node() = default;

  virtual const ChildList* Children() const
  {
    return nullptr;
  }
  virtual ChildList* Children()
  {
    return nullptr;
  }

  virtual bool childrenVisible() const
  {
    return true;
  }

  Node* Parent() const
  {
    return parent;
  }

  virtual bwLayoutInterface* Layout() const
  {
    return nullptr;
  }

  virtual bwWidget* Widget() const
  {
    return nullptr;
  }

  EventHandler* eventHandler() const
  {
    return handler.get();
  }

  virtual bwRectanglePixel Rectangle() const = 0;
  virtual std::optional<bwRectanglePixel> MaskRectangle() const = 0;
  virtual bool isVisible() const = 0;

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
  const ChildList* Children() const override
  {
    return &children;
  }
  ChildList* Children() override
  {
    return &children;
  }

  bwLayoutInterface* Layout() const override
  {
    return layout.get();
  }

  bwRectanglePixel Rectangle() const override
  {
    return layout->getRectangle();
  }

  std::optional<bwRectanglePixel> MaskRectangle() const override
  {
    return std::nullopt;
  }

  bool isVisible() const override
  {
    return true;
  }

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
  bwWidget* Widget() const override
  {
    return &*widget;
  }

  bwRectanglePixel Rectangle() const override
  {
    return widget->rectangle;
  }

  std::optional<bwRectanglePixel> MaskRectangle() const override
  {
    return std::nullopt;
  }

  bool isVisible() const override
  {
    return widget->hidden == false;
  }

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
  const ChildList* Children() const override
  {
    return LayoutNode::Children();
  }
  ChildList* Children() override
  {
    return LayoutNode::Children();
  }

  bwLayoutInterface* Layout() const override
  {
    return LayoutNode::Layout();
  }

  bwWidget* Widget() const override
  {
    return WidgetNode::Widget();
  }

  bwContainerWidget& ContainerWidget() const
  {
    return static_cast<bwContainerWidget&>(*Widget());
  }

  bwRectanglePixel Rectangle() const override
  {
    return WidgetNode::Rectangle();
  }
  bwRectanglePixel ContentRectangle() const
  {
    return LayoutNode::Rectangle();
  }

  std::optional<bwRectanglePixel> MaskRectangle() const override
  {
    return ContainerWidget().getMaskRectangle();
  }

  bool isVisible() const override
  {
    return WidgetNode::isVisible();
  }

  bool childrenVisible() const override
  {
    return ContainerWidget().childrenVisible();
  }
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
