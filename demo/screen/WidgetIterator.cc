/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Original work Copyright (c) 2018 Julian Eisel
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include "WidgetIterator.h"

using namespace bWidgetsDemo;


WidgetIterator::WidgetIterator(
        const LayoutItem* layout_item,
        const LayoutItem* subtree_root,
        const bool skip_hidden) :
    item(layout_item), subtree_root(subtree_root), skip_hidden(skip_hidden)
{
}

WidgetIteratorWithHidden WidgetIterator::withHidden(const LayoutItem& layout_item)
{
	return WidgetIteratorWithHidden(layout_item);
}

WidgetIterator& WidgetIterator::operator=(const WidgetIterator& other)
{
	item = other.item;
	return *this;
}

bool WidgetIterator::operator!=(const WidgetIterator& other) const
{
	return item != other.item;
}

static bool should_skip_item(const LayoutItem& layout_item, const bool skip_hidden)
{
	return skip_hidden && layout_item.isHidden();
}

WidgetIterator WidgetIterator::begin_intern(
        const LayoutItem& layout_item,
        const LayoutItem& subtree_root,
        const bool skip_hidden)
{
	if (layout_item.getWidget() && !should_skip_item(layout_item, skip_hidden)) {
		return WidgetIterator(&layout_item, &subtree_root, skip_hidden);
	}

	for (const auto& iter_item : layout_item.child_items) {
		WidgetIterator iter_result = begin_intern(*iter_item, subtree_root, skip_hidden);
		if (iter_result != end(*iter_item)) {
			return iter_result;
		}
	}

	return end(layout_item);
}

WidgetIterator WidgetIterator::operator++()
{
	// First traverse down the tree.
	for (auto& layout_item : item->child_items) {
		WidgetIterator iter_result = begin_intern(*layout_item, *subtree_root, skip_hidden);
		if (iter_result != end(*layout_item)) {
			*this = iter_result;
			return *this;
		}
	}

	// Next traverse neighbours and up the tree, until we're at the initial item the iteration was started from.
	for (const LayoutItem* layout_item = item; layout_item && layout_item != subtree_root; layout_item = layout_item->parent) {
		for (auto neighbour = std::next(layout_item->iterator_item);
		     layout_item->parent && neighbour != layout_item->parent->child_items.end();
		     ++neighbour)
		{
			const WidgetIterator& iter_result = begin_intern(**neighbour, *subtree_root, skip_hidden);
			if (iter_result != end(**neighbour)) {
				*this = iter_result;
				return *this;
			}
		}
	}

	*this = end(*item);
	return *this;
}

bWidgets::bwWidget& WidgetIterator::operator*()
{
	assert(item->getWidget());
	return *item->getWidget();
}

namespace bWidgetsDemo {

WidgetIterator begin(const LayoutItem& layout_item)
{
	return WidgetIterator::begin_intern(layout_item, layout_item, true);
}

WidgetIterator end(const LayoutItem&)
{
	return WidgetIterator(nullptr, nullptr, true);
}

WidgetIteratorWithHidden::WidgetIteratorWithHidden(const LayoutItem& layout_item) :
    layout_item(layout_item)
{
	
}

WidgetIterator begin(const WidgetIteratorWithHidden& iterator)
{
	return WidgetIterator::begin_intern(iterator.layout_item, iterator.layout_item, false);
}

WidgetIterator end(const WidgetIteratorWithHidden&)
{
	return WidgetIterator(nullptr, nullptr, false);
}

} // namespace bWidgetsDemo
