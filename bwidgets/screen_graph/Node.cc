#include "bwContainerWidget.h"
#include "bwLayoutInterface.h"
#include "bwWidget.h"
#include "screen_graph/EventHandler.h"

#include "Node.h"

namespace bWidgets::bwScreenGraph {

Node::~Node()
{
  handler = nullptr;
}

auto Node::Children() const -> const Node::ChildList*
{
  return nullptr;
}

auto Node::Children() -> Node::ChildList*
{
  return nullptr;
}

auto Node::childrenVisible() const -> bool
{
  return true;
}

auto Node::Layout() const -> bwLayoutInterface*
{
  return nullptr;
}

auto Node::Widget() const -> bwWidget*
{
  return nullptr;
}

auto Node::Parent() const -> Node*
{
  return parent;
}

auto Node::eventHandler() const -> EventHandler*
{
  return handler.get();
}

void Node::moveState(Node& from)
{
  std::swap(handler, from.handler);
  handler->node = this;
}

/* -------------------------------------------------------------------- */
/* LayoutNode */

LayoutNode::~LayoutNode()
{
  layout = nullptr;
}

auto LayoutNode::Children() const -> const ChildList*
{
  return &children;
}
auto LayoutNode::Children() -> ChildList*
{
  return &children;
}

auto LayoutNode::Layout() const -> bwLayoutInterface*
{
  return layout.get();
}

auto LayoutNode::Rectangle() const -> bwRectanglePixel
{
  return layout->getRectangle();
}

auto LayoutNode::MaskRectangle() const -> std::optional<bwRectanglePixel>
{
  return std::nullopt;
}

auto LayoutNode::isVisible() const -> bool
{
  return true;
}

auto LayoutNode::matches(const Node&) -> bool const
{
  /* Layouts can not be uniquely identified. They do not store state that would have to be kept
   * over redraws, so they can be entirely reconstructed on each redraw. */
  return false;
}

/* -------------------------------------------------------------------- */
/* WidgetNode */

auto WidgetNode::Widget() const -> bwWidget*
{
  return &*widget;
}

auto WidgetNode::Rectangle() const -> bwRectanglePixel
{
  return widget->rectangle;
}

auto WidgetNode::MaskRectangle() const -> std::optional<bwRectanglePixel>
{
  return std::nullopt;
}

auto WidgetNode::isVisible() const -> bool
{
  return widget->isHidden() == false;
}

auto WidgetNode::matches(const Node& other) -> bool const
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

void WidgetNode::moveState(Node& from)
{
  Node::moveState(from);
  if (bwWidget* other_widget = from.Widget()) {
    widget->copyState(*other_widget);
  }
}

/* -------------------------------------------------------------------- */
/* ContainerNode */

auto ContainerNode::Children() const -> const ChildList*
{
  return LayoutNode::Children();
}
auto ContainerNode::Children() -> ChildList*
{
  return LayoutNode::Children();
}

auto ContainerNode::Layout() const -> bwLayoutInterface*
{
  return LayoutNode::Layout();
}

auto ContainerNode::Widget() const -> bwWidget*
{
  return WidgetNode::Widget();
}

auto ContainerNode::ContainerWidget() const -> bwContainerWidget&
{
  return static_cast<bwContainerWidget&>(*Widget());
}

auto ContainerNode::Rectangle() const -> bwRectanglePixel
{
  return WidgetNode::Rectangle();
}
auto ContainerNode::ContentRectangle() const -> bwRectanglePixel
{
  return LayoutNode::Rectangle();
}

auto ContainerNode::MaskRectangle() const -> std::optional<bwRectanglePixel>
{
  return ContainerWidget().getMaskRectangle();
}

auto ContainerNode::isVisible() const -> bool
{
  return WidgetNode::isVisible();
}

auto ContainerNode::childrenVisible() const -> bool
{
  return ContainerWidget().childrenVisible();
}

auto ContainerNode::matches(const Node& other) -> bool const
{
  return WidgetNode::matches(other);
}

void ContainerNode::moveState(Node& from)
{
  WidgetNode::moveState(from);
}

}  // namespace bWidgets::bwScreenGraph
