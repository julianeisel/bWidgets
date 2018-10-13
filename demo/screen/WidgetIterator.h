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

#pragma once

#include <iterator>

#include "Layout.h"


namespace bWidgets {
class bwWidget;
}

namespace bWidgetsDemo {

class WidgetIteratorWithHidden
{
	friend class WidgetIterator begin(const WidgetIteratorWithHidden&);
	friend class WidgetIterator end(const WidgetIteratorWithHidden&);
	friend class WidgetIterator;

private:
	WidgetIteratorWithHidden(const LayoutItem&);
	WidgetIteratorWithHidden(const WidgetIteratorWithHidden&) = default;

	const LayoutItem& layout_item;
};

class WidgetIterator : public std::iterator<std::forward_iterator_tag, bWidgets::bwWidget>
{
	friend WidgetIterator begin(const LayoutItem&);
	friend WidgetIterator end(const LayoutItem&);
	friend WidgetIterator begin(const WidgetIteratorWithHidden&);
	friend WidgetIterator end(const WidgetIteratorWithHidden&);
	friend class WidgetIteratorWithHidden;

public:
	WidgetIterator() = delete;
	WidgetIterator(const WidgetIterator&) = default;
	WidgetIterator(WidgetIterator&& other) = default;

	static WidgetIteratorWithHidden withHidden(const LayoutItem&);

	WidgetIterator& operator=(const WidgetIterator&);
	bool operator==(const WidgetIterator&) const;
	bool operator!=(const WidgetIterator&) const;
	WidgetIterator operator++();
	const bWidgets::bwWidget& operator*() const;
	bWidgets::bwWidget& operator*();

private:
	static WidgetIterator begin_intern(
	        const LayoutItem &, const LayoutItem &,
	        const bool skip_hidden);
	explicit WidgetIterator(const LayoutItem*, const LayoutItem*, const bool skip_hidden);

	const LayoutItem* item;
	const LayoutItem* subtree_root;
	bool skip_hidden;
};

WidgetIterator begin(const LayoutItem&);
WidgetIterator end(const LayoutItem&);
WidgetIterator begin(const WidgetIteratorWithHidden&);
WidgetIterator end(const WidgetIteratorWithHidden&);

} // namespace bWidgetsDemo
