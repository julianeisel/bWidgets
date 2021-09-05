#include <typeinfo>

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

auto Node::matchesType(const Node& other) const -> bool
{
  return typeid(*this) == typeid(other);
}

void Node::moveState(Node& from)
{
  if (!Node::matchesType(from)) {
    return;
  }
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

auto LayoutNode::matchesType(const Node& other) const -> bool
{
  if (!Node::matchesType(other)) {
    return false;
  }
  bwLayoutInterface* this_layout = layout.get();
  bwLayoutInterface* other_layout = other.Layout();
  return typeid(this_layout) == typeid(other_layout);
}

auto LayoutNode::matches(const Node&) const -> bool
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

auto WidgetNode::matchesType(const Node& other) const -> bool
{
  if (!Node::matchesType(other)) {
    return false;
  }
  bwWidget* this_widget = widget.get();
  bwWidget* other_widget = other.Widget();
  assert(widget != nullptr);
  assert(other_widget != nullptr);
  return typeid(*this_widget) == typeid(*other_widget);
}

auto WidgetNode::matches(const Node& other) const -> bool
{
  /* Compare node and widget types. */
  if (!matchesType(other)) {
    return false;
  }
  bwWidget* other_widget = other.Widget();

  if (widget.get() == other_widget) {
    /* Quick path: Same address, no need to check further. */
    return true;
  }

  /* Compares the actual widgets, i.e. calls the widget's custom matches() overload.
   * Crucial for identifying widgets over redraws and preserving state that way. */
  return widget->matches(*other_widget);
}

/**
 * Move a node's state and its widget's state to another node of the same type. Matching means that
 * the other
 */
void WidgetNode::moveState(Node& from)
{
  if (!matchesType(from)) {
    return;
  }

  Node::moveState(from);
  widget->copyState(*from.Widget());
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

auto ContainerNode::matchesType(const Node& other) const -> bool
{
  return LayoutNode::matchesType(other) && WidgetNode::matchesType(other);
}

auto ContainerNode::matches(const Node& other) const -> bool
{
  return WidgetNode::matches(other);
}

void ContainerNode::moveState(Node& from)
{
  WidgetNode::moveState(from);
}

}  // namespace bWidgets::bwScreenGraph
