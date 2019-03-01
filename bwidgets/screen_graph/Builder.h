#pragma once

#include "bwPtr.h"
#include "bwWidget.h"
#include "Node.h"

namespace bWidgets {
namespace bwScreenGraph {

/**
 * \brief Helper class to construct screen-graphs.
 */
class Builder
{
public:
	Builder() = default;
	Builder(Node& active_layout_node);
	virtual ~Builder() = default;

	static Node& addChildNode(Node& node, bwPtr<Node> new_child_node);
	static void setLayout(Node& node, bwPtr<bwLayoutInterface> layout);
	static bwWidget& addWidget(Node& node, bwPtr<bwWidget> widget);

	void setActiveLayout(bwScreenGraph::Node&);

	/**
	 * \brief Add child layout for a layout created in-place.
	 */
	template<typename _LayoutType, typename... _Args>
	Node& addLayout(_Args&&... __args)
	{
		Node& new_node = addChildNode(*_active_layout_node, bwPtr_new<Node>());
		new_node.layout = bwPtr_new<_LayoutType>(std::forward<_Args>(__args)...);
		return new_node;
	}

	template<typename _WidgetType, typename... _Args>
	_WidgetType& addWidget(_Args&&... __args)
	{
		Node& new_node = addChildNode(*_active_layout_node, bwPtr_new<Node>());
		new_node.widget = bwPtr_new<_WidgetType>(std::forward<_Args>(__args)...);
		return static_cast<_WidgetType&>(*new_node.widget);
	}

	/**
	 * \brief Add child node for a widget created in-place.
	 * Simplifies creating and appending widgets. The arguments \a __args are
	 * forwarded to the widget constructor; a new node for it is added as child
	 * to \a node.
	 *
	 * Without `emplaceWidget()`:
	 * \code
	 * using bWidgets::bwScreenGraph;
	 *
	 * auto& widget = static_cast<bwLabel&>(Builder::addWidget<bwLabel>(screen_graph, bwPtr_new<bwLabel>("Foo", 0, 10)));
	 * widget.foo();
	 * // ...
	 * \endcode
	 *
	 * With `emplaceWidget()`:
	 * \code
	 * using bWidgets::bwScreenGraph;
	 *
	 * auto& widget = Builder::emplaceWidget<bwLabel>(screen_graph, "Foo", 0, 10);
	 * widget.foo();
	 * // ...
	 * \endcode
	 */
	template<typename _WidgetType, typename... _Args>
	static _WidgetType& emplaceWidget(Node& node, _Args&&... __args)
	{
		Node& new_node = addChildNode(node, bwPtr_new<Node>());
		new_node.widget = bwPtr_new<_WidgetType>(std::forward<_Args>(__args)...);
		return static_cast<_WidgetType&>(*new_node.widget);
	}

//private: // Public for container widgets. We should add proper support for these here.
	static void setWidget(Node& node, bwPtr<bwWidget> widget);

private:
	bwScreenGraph::Node* _active_layout_node{nullptr};
};

} // namespace bwScreenGraph
} // namespace bWidgets
