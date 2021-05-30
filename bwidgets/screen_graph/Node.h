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

  virtual auto Children() const -> const ChildList*
  {
    return nullptr;
  }
  virtual auto Children() -> ChildList*
  {
    return nullptr;
  }

  virtual auto childrenVisible() const -> bool
  {
    return true;
  }

  virtual auto Layout() const -> bwLayoutInterface*
  {
    return nullptr;
  }

  virtual auto Widget() const -> bwWidget*
  {
    return nullptr;
  }

  auto Parent() const -> Node*
  {
    return parent;
  }

  auto eventHandler() const -> EventHandler*
  {
    return handler.get();
  }

  virtual auto Rectangle() const -> bwRectanglePixel = 0;
  virtual auto MaskRectangle() const -> std::optional<bwRectanglePixel> = 0;
  virtual auto isVisible() const -> bool = 0;
  virtual auto matches(const Node&) -> bool const = 0;

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
  auto Children() const -> const ChildList* override
  {
    return &children;
  }
  auto Children() -> ChildList* override
  {
    return &children;
  }

  auto Layout() const -> bwLayoutInterface* override
  {
    return layout.get();
  }

  auto Rectangle() const -> bwRectanglePixel override
  {
    return layout->getRectangle();
  }

  auto MaskRectangle() const -> std::optional<bwRectanglePixel> override
  {
    return std::nullopt;
  }

  auto isVisible() const -> bool override
  {
    return true;
  }

  auto matches(const Node&) -> bool const override
  {
    /* Layouts can not be uniquely identified. They do not store state that would have to be kept
     * over redraws, so they can be entirely reconstructed on each redraw. */
    return false;
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
  auto Widget() const -> bwWidget* override
  {
    return &*widget;
  }

  auto Rectangle() const -> bwRectanglePixel override
  {
    return widget->rectangle;
  }

  auto MaskRectangle() const -> std::optional<bwRectanglePixel> override
  {
    return std::nullopt;
  }

  auto isVisible() const -> bool override
  {
    return widget->isHidden() == false;
  }

  auto matches(const Node& other) -> bool const override
  {
    bwWidget* other_widget = other.Widget();
    if (!other_widget) {
      return false;
    }

    return (widget.get() == other_widget) ||
           /* Compares the actual widgets, not the pointers. I.e. calls the widget's custom
            * matches( overload. Crucial for identifying widgets over redraws and preserving
            * state that way. */
           (widget->matches(*other_widget));
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
  auto Children() const -> const ChildList* override
  {
    return LayoutNode::Children();
  }
  auto Children() -> ChildList* override
  {
    return LayoutNode::Children();
  }

  auto Layout() const -> bwLayoutInterface* override
  {
    return LayoutNode::Layout();
  }

  auto Widget() const -> bwWidget* override
  {
    return WidgetNode::Widget();
  }

  auto ContainerWidget() const -> bwContainerWidget&
  {
    return static_cast<bwContainerWidget&>(*Widget());
  }

  auto Rectangle() const -> bwRectanglePixel override
  {
    return WidgetNode::Rectangle();
  }
  auto ContentRectangle() const -> bwRectanglePixel
  {
    return LayoutNode::Rectangle();
  }

  auto MaskRectangle() const -> std::optional<bwRectanglePixel> override
  {
    return ContainerWidget().getMaskRectangle();
  }

  auto isVisible() const -> bool override
  {
    return WidgetNode::isVisible();
  }

  auto childrenVisible() const -> bool override
  {
    return ContainerWidget().childrenVisible();
  }

  auto matches(const Node& other) -> bool const override
  {
    return WidgetNode::matches(other);
  }
};

}  // namespace bwScreenGraph
}  // namespace bWidgets
