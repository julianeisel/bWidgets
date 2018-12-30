#include <cassert>

#include "Builder.h"

using namespace bWidgets;
using namespace bWidgets::bwScreenGraph;


Node& Builder::addChildNode(
        Node& node,
        bwPtr<Node> new_child_node)
{
	node.children.push_back(std::move(new_child_node));
	return *node.children.back();
}

void Builder::setLayout(
        Node& node,
        bwPtr<bwLayoutInterface> layout)
{
	node.layout = std::move(layout);
}

void Builder::setWidget(
        Node& node,
        bwPtr<bwWidget> widget)
{
	node.widget = std::move(widget);
}

bwWidget& Builder::addWidget(
        Node& node,
        bwPtr<bwWidget> widget)
{
	auto new_node = bwPtr_new<Node>();
	setWidget(*new_node, std::move(widget));
	Node& node_ref = addChildNode(node, std::move(new_node));
	return *node_ref.widget;
}
