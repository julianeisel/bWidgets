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
	virtual ~Builder() = default;

	static Node& addChildNode(Node& node, bwPtr<Node> new_child_node);
	static void setLayout(Node& node, bwPtr<bwLayoutInterface> layout);
	static bwWidget& addWidget(Node& node, bwPtr<bwWidget> widget);

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
		auto new_node = bwPtr_new<Node>();
		new_node->widget = bwPtr_new<_WidgetType>(std::forward<_Args>(__args)...);
		Node& node_ref = addChildNode(node, std::move(new_node));
		return static_cast<_WidgetType&>(*node_ref.widget);
	}

private:
	static void setWidget(Node& node, bwPtr<bwWidget> widget);
};

} // namespace bwScreenGraph
} // namespace bWidgets
