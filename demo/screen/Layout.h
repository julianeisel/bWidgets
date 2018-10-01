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

#include <list>

#include "bwUtil.h"


namespace bWidgets {
	class bwPanel;
	class bwPoint;
	class bwStyle;
	class bwWidget;
}

namespace bWidgetsDemo {

/**
 * \brief An abstract class for defining items that form the layout.
 *
 * The LayoutItem class provides needed functionality to define and draw widgets,
 * which get automatically placed and sized based on the layout-item type.
 * Each item can have its own sub-items with different types.
 *
 * To update position and sizes of an item and it's sub-items, call resolve(). Call
 * draw() to draw the widgets based on positions and sizes resolve() calculated.
 * Normally, draw() is called immediately after resolve().
 *
 * \note This currently is rather basic and meant for testing bWidgets mainly. It's
 *       planned to give bWidgets its own layout-engine at some point.
 * \note We currently expect a RootLayout to be at the top of the tree. Through it
 *       the needed position and size hints can be set. Without changing these, the
 *       calculated widget-coordinates don't change.
 */
class LayoutItem
{
public:
	enum LayoutItemType {
		LAYOUT_ITEM_TYPE_ROOT,
		LAYOUT_ITEM_TYPE_ROW,
		LAYOUT_ITEM_TYPE_COLUMN,
		LAYOUT_ITEM_TYPE_PANEL,
		LAYOUT_ITEM_TYPE_WIDGET,
	};

	enum FlowDirection {
		// Widgets and child-layouts are added top to down
		FLOW_DIRECTION_VERTICAL,
		// Widgets and child-layouts are added left to right
		FLOW_DIRECTION_HORIZONTAL,
	};

	virtual ~LayoutItem() = default;

	bool iterateWidgets(
	        bool (*callback)(bWidgets::bwWidget& widget, void* custom_data),
	        void* custom_data,
	        bool skip_hidden = false);

	virtual void draw(bWidgets::bwStyle& style) const;
	virtual void resolve(
	        const bWidgets::bwPoint& layout_pos,
	        const unsigned int item_margin,
	        const float scale_fac,
	        const LayoutItem& parent);
	virtual bWidgets::bwOptional<std::reference_wrapper<bWidgets::bwWidget>> getWidget() const;
	virtual bool isHidden() const;
	virtual bool areChildrenHidden() const;

	void addWidget(bWidgets::bwPtr<bWidgets::bwWidget> widget);

	bool hasChild(const LayoutItem& potential_child) const;

	unsigned int getHeight() const;

	const LayoutItemType type;
	const FlowDirection flow_direction;
	const bool align;
	void addLayoutItem(bWidgets::bwPtr<LayoutItem> layout_item);

protected:
	// Protected constructor to force calling through inherited class (pseudo abstract).
	LayoutItem(
	        LayoutItemType layout_type,
	        const bool align,
	        FlowDirection flow_direction = FLOW_DIRECTION_HORIZONTAL);

	bWidgets::bwOptional<std::reference_wrapper<LayoutItem>> getPrevious(
	        const LayoutItem& parent,
	        const bool skip_hidden) const;
	bWidgets::bwOptional<std::reference_wrapper<LayoutItem>> getNext(
	        const LayoutItem& parent,
	        const bool skip_hidden) const;

	using LayoutItemList = std::list<bWidgets::bwPtr<LayoutItem>>;
	using IteratorItem = LayoutItemList::const_iterator;

	// The iterator-wrapper for this item, stored to avoid lookups.
	// Only valid when item was assigned to a parent using addLayoutItem.
	IteratorItem iterator_item;

	unsigned int width{0}, height{0};
	// Properties defined on layout-item creation, not meant to be modified later on.
	LayoutItemList child_items;

private:
	bool needsMarginBeforeNext(const LayoutItem& parent) const;
	unsigned int countRowColumns() const;
	unsigned int countNeededMargins() const;
};


/**
 * \brief The layout item at the root of the layout-item tree.
 *
 * A RootLayout can be used to manage child-layouts more easily.
 *
 * \note Only root-layouts with \a flow_direction FLOW_DIRECTION_VERTICAL are supported right now.
 */
class RootLayout : public LayoutItem
{
public:
	RootLayout(
//	        FlowDirection direction,
	        const int ymax, const unsigned int max_size,
	        const bool align = false);

	void resolve(
	        const float vertical_scroll,
	        const float scale_fac);
	void setMaxSize(const unsigned int max_size);
	void setYmax(const int value);

	unsigned int padding;
	unsigned int item_margin;

private:
	unsigned int max_size;
	int ymax;
};


class ColumnLayout : public LayoutItem
{
public:
	static ColumnLayout& create(
	        LayoutItem& parent,
	        const bool align);
private:
	explicit ColumnLayout(const bool align = false);
};


class RowLayout : public LayoutItem
{
public:
	static RowLayout& create(
	        LayoutItem& parent,
	        const bool align = false);
private:
	explicit RowLayout(const bool align = false);
};


class PanelLayout : public LayoutItem
{
public:
	static PanelLayout& create(
	        const std::string& label,
	        unsigned int header_height_hint,
	        LayoutItem& parent);

	void draw(bWidgets::bwStyle& style) const override;
	void resolve(
	        const bWidgets::bwPoint& layout_pos,
	        const unsigned int item_margin,
	        const float scale_fac,
	        const LayoutItem& parent) override;
	bWidgets::bwOptional<std::reference_wrapper<bWidgets::bwWidget>> getWidget() const override;
	bool areChildrenHidden() const override;

private:
	explicit PanelLayout(
	        const std::string& label,
	        unsigned int header_height_hint);

	void resolveContent(
	        const bWidgets::bwPoint& panel_pos,
	        const unsigned int padding,
	        const unsigned int item_margin,
	        const float scale_fac);

	bWidgets::bwPtr<class bWidgets::bwPanel> panel;
};

} // namespace bWidgetsDemo
