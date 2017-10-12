#pragma once

#include <list>
#include "bwWidget.h"

class WidgetLayoutItem;

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
		LAYOUT_ITEM_TYPE_WIDGET,
	};

	enum FlowDirection {
		// Widgets and child-layouts are added top to down
		FLOW_DIRECTION_VERTICAL,
		// Widgets and child-layouts are added left to right
		FLOW_DIRECTION_HORIZONTAL,
	};

	virtual ~LayoutItem();

	void resolve(
	        const bWidgets::bwPoint& layout_pos,
	        const unsigned int item_margin,
	        const float scale_fac);
	void draw(bWidgets::bwStyle& style);
	bool iterateWidgets(bool (*callback)(bWidgets::bwWidget& widget, void* custom_data), void* custom_data);

	void addWidget(bWidgets::bwWidget* widget);
	void addLayout(class RowLayout* layout);
	void addLayout(class ColumnLayout* column);

	bool hasChild(const LayoutItem& potential_child) const;

	unsigned int getHeight() const;

	const LayoutItemType type;
	const FlowDirection flow_direction;
	const bool align;

protected:
	// Protected constructor to force calling through inherited class (pseudo abstract).
	LayoutItem(
	        LayoutItemType layout_type,
	        const bool align,
	        LayoutItem *parent = nullptr,
	        FlowDirection flow_direction = FLOW_DIRECTION_HORIZONTAL);

	LayoutItem* getPrevious(const LayoutItem& parent) const;
	LayoutItem* getNext(const LayoutItem& parent) const;

	typedef std::list<LayoutItem*>::const_iterator IteratorItem;

	// The iterator-wrapper for this item, stored to avoid lookups.
	// Only valid when item was assigned to a parent using addLayout.
	IteratorItem iterator_item;

	unsigned int width, height;
	// Properties defined on layout-item creation, not meant to be modified later on.
	std::list<LayoutItem*> child_items;

private:
	void addLayoutItem(LayoutItem* layout);
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
	explicit ColumnLayout(
	        const bool align = false,
	        LayoutItem *parent = nullptr);
};


class RowLayout : public LayoutItem
{
public:
	explicit RowLayout(
	        const bool align = false,
	        LayoutItem *parent = nullptr);
};

} // namespace bWidgetsDemo
