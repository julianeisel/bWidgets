#include <cassert>

#include "Builder.h"

namespace bWidgets {
namespace bwScreenGraph {

Builder::Builder(LayoutNode& active_layout_node) : _active_layout_node(&active_layout_node)
{
}

void Builder::setLayout(LayoutNode& node, bwPtr<bwLayoutInterface> layout)
{
  node.layout = std::move(layout);
}

void Builder::setWidget(WidgetNode& node, bwPtr<bwWidget> widget)
{
  node.widget = std::move(widget);
  node.handler = node.widget->createHandler();
}

bwWidget& Builder::addWidget(LayoutNode& node, bwPtr<bwWidget> widget)
{
  WidgetNode& node_ref = addChildNode<WidgetNode>(node);
  setWidget(node_ref, std::move(widget));
  return *node_ref.widget;
}

void Builder::setActiveLayout(LayoutNode& node)
{
  _active_layout_node = &node;
}

}  // namespace bwScreenGraph
}  // namespace bWidgets
