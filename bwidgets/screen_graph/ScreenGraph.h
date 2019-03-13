#pragma once

#include "bwContext.h"
#include "bwPtr.h"

namespace bWidgets {
namespace bwScreenGraph {

class Node;

/* Template parameter is type of the root node. */
template<typename _RootNodeType>
class ScreenGraph
{
	static_assert(std::is_base_of<Node, _RootNodeType>::value, "Should derrive from bwScreenGraph::Node");

public:
	/** The context describing the state of this screen-graph */
	bwContext context;

	ScreenGraph(bwPtr<_RootNodeType> _root_node) :
	    root_node(std::move(_root_node))
	{}

	_RootNodeType& Root() const
	{
		return *root_node;
	}

private:
	bwPtr<_RootNodeType> root_node;
};

} // namespace bwScreenGraph
} // namespace bWidgets
