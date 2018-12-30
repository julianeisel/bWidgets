#pragma once

#include <list>

#include "bwLayoutInterface.h"
#include "bwPtr.h"
#include "bwWidget.h"


namespace bWidgets {
namespace bwScreenGraph {

struct Node
{
	friend class Builder;

	Node() = default;

private:
	std::list<bwPtr<Node>> children;

	bwPtr<bwLayoutInterface> layout;
	bwPtr<bwWidget> widget;
};

} // namespace bwScreenGraph
} // namespace bWidgets
