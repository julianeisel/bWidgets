#include <cassert>

#include "Builder.h"

namespace bWidgets {
namespace bwScreenGraph {

Builder::Builder(LayoutNode& active_layout_node) : _active_layout_node(active_layout_node)
{
}

Builder::Builder(ScreenGraph& screen_graph) : Builder(screen_graph.Root())
{
}

void Builder::setLayout(LayoutNode& node, std::unique_ptr<bwLayoutInterface> layout)
{
  node.layout = std::move(layout);
}

void Builder::setWidget(WidgetNode& node, std::unique_ptr<bwWidget> widget)
{
  node.widget = std::move(widget);
  node.handler = node.widget->createHandler();
}

auto Builder::addWidget(LayoutNode& node, std::unique_ptr<bwWidget> widget) -> bwWidget&
{
  WidgetNode& node_ref = addChildNode<WidgetNode>(node);
  setWidget(node_ref, std::move(widget));
  return *node_ref.widget;
}

/**
 * \brief Activate a layout node.
 *
 * Adding further items will add them as children to this node, until a
 * different layout node is activated.
 */
void Builder::setActiveLayout(LayoutNode& node)
{
  _active_layout_node = node;
}

}  // namespace bwScreenGraph
}  // namespace bWidgets
