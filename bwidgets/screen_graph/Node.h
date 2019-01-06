#pragma once

#include <list>

#include "bwLayoutInterface.h"
#include "bwPtr.h"
#include "bwWidget.h"


namespace bWidgets {
namespace bwScreenGraph {

/**
 * \brief The data-structure for a screen-graph node
 *
 * Screen-graph nodes are the atomical components of a screen-graph, which is
 * the key for bWidgets' design. If you're not familiar with our screen-graph
 * concept, you should really have a look at the high level bWidgets
 * documentation [TODO link].
 *
 * To ensure separation of concerns, the screen-graph node must not have any
 * functionality of its own (no member functions). Any functionality must be
 * implemented using friend classes and functions. For example to build a
 * screen-graph and the necessary nodes, a screen-graph builder class should be
 * used and be declared as friend of the `Node` class.
 * Having to declare those helpers as friends may turn out to an annoyance
 * with small benefits. In that case we should just make data public.
 *
 * TL;DR: screen-graph nodes are supposed to stay pure/mere data structures.
 */
struct Node
{
	friend class Builder;
	friend class PreOrderIterator;

	using ChildList = std::list<bwPtr<Node>>;
	using ChildIterator = ChildList::iterator;

	Node() = default;
	~Node() = default;

	const ChildList& Children() const
	{
		return children;
	}
	ChildList& Children()
	{
		return children;
	}

	const Node* Parent() const
	{
		return parent;
	}

	bwLayoutInterface* Layout() const
	{
		return &*layout;
	}

	bwWidget* Widget() const
	{
		return &*widget;
	}

private:
	ChildList children;
	Node* parent;

	bwPtr<bwLayoutInterface> layout;
	bwPtr<bwWidget> widget;
};

} // namespace bwScreenGraph
} // namespace bWidgets
