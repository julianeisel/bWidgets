#include <algorithm>
#include <cassert>

#include "bwAbstractButton.h"
#include "bwPainter.h"
#include "bwPoint.h"

#include "Layout.h"

using namespace bWidgetsDemo;
using namespace bWidgets; // Less verbose


/**
 * \brief Layout-item wrapping a widget to place it within a parent layout.
 */
class WidgetLayoutItem : public LayoutItem
{
public:
	WidgetLayoutItem(bwWidget& widget, LayoutItem* parent = nullptr);

	void fitWidgetToItem(
	        const int xmin, const int ymax) const;

	void alignWidgetItem(const LayoutItem& parent);
	bool canAlignWidgetItem() const;
	void alignmentSanityCheck(const LayoutItem& parent) const;
	// These could easily share code with needsMarginBeforeNext(). But it works now, so no touching ;).
	bool isAlignedtoPrevious(const LayoutItem& parent) const;
	bool isAlignedtoNext(const LayoutItem& parent) const;

	bwWidget& widget;
};


LayoutItem::LayoutItem(
        LayoutItemType item_type,
        const bool align,
        LayoutItem *parent,
        FlowDirection flow_direction) :
    type(item_type), flow_direction(flow_direction), align(align)
{
	if (parent) {
		assert(parent->hasChild(*this) == false);
		parent->addLayoutItem(this);
	}
}

LayoutItem::~LayoutItem()
{
	for (LayoutItem* item : child_items) {
		delete item;
	}
}

void LayoutItem::draw(bwStyle& style)
{
	for (LayoutItem* item : child_items) {
		if (item->type == LAYOUT_ITEM_TYPE_WIDGET) {
			WidgetLayoutItem& widget_item = *static_cast<WidgetLayoutItem*>(item);

			if (!widget_item.widget.rectangle.isEmpty()) {
				widget_item.widget.draw(style);
			}
		}
		else {
			item->draw(style);
		}
	}
}

bool LayoutItem::iterateWidgets(bool (*callback)(bwWidget&, void*), void* custom_data)
{
	for (LayoutItem* item : child_items) {
		if (item->type == LAYOUT_ITEM_TYPE_WIDGET) {
			WidgetLayoutItem& widget_item = *static_cast<WidgetLayoutItem*>(item);

			if (!callback(widget_item.widget, custom_data)) {
				return false;
			}
		}
		else if (!item->iterateWidgets(callback, custom_data)) {
			return false;
		}
	}

	return true;
}

void LayoutItem::addWidget(bwWidget* widget)
{
	WidgetLayoutItem* widget_item = new WidgetLayoutItem(*widget);
	addLayoutItem(widget_item);
}

void LayoutItem::addLayoutItem(LayoutItem* item)
{
	child_items.push_back(item);
	item->iterator_item = std::prev(child_items.end());
}

void LayoutItem::addLayout(RowLayout* row)
{
	addLayoutItem(row);
}

void LayoutItem::addLayout(ColumnLayout* column)
{
	addLayoutItem(column);
}

bool LayoutItem::hasChild(const LayoutItem& potential_child) const
{
	return std::find(child_items.begin(), child_items.end(), &potential_child) != child_items.end();
}

unsigned int LayoutItem::getHeight() const
{
	return height;
}

void LayoutItem::resolve(
        const bWidgets::bwPoint& layout_pos,
        const unsigned int item_margin,
        const float scale_fac)
{
	int xpos = layout_pos.x;
	int ypos = layout_pos.y;
	unsigned int item_width = 0;
	// Width calculations may have imprecisions so widths don't add up to full layout width. We 'fix' this
	// by adding a pixel to each layout-item until the remainder is 0, meaning total width matches parent
	// width precisely. Also makes layout less jaggy on window size changes.
	int additional_remainder_x = 0;

	height = 0;

	if (flow_direction == FLOW_DIRECTION_HORIZONTAL) {
		// The max-width of each item is the max-width of the parent, divided by the count of its
		// horizontally placed sub-items. In other words, each item has the same max-width that
		// add up to the parent's max-width. From this max-width, the item margins are subtracted.
		unsigned int tot_row_cols = countRowColumns();

		if (tot_row_cols != 0) {
			item_width = (width - (countNeededMargins() * item_margin)) / tot_row_cols;
		}
		additional_remainder_x = width - ((item_width * tot_row_cols) + (countNeededMargins() * item_margin));
		assert(additional_remainder_x >= 0);
	}
	else {
		item_width = width;
	}

	for (LayoutItem* item : child_items) {
		item->width = item_width;
		// Simple correction for precision issues.
		if (additional_remainder_x > 0) {
			item->width++;
			additional_remainder_x--;
		}

		if (item->type == LAYOUT_ITEM_TYPE_WIDGET) {
			WidgetLayoutItem& widget_item = *static_cast<WidgetLayoutItem*>(item);
			bwWidget& widget = widget_item.widget;

			widget_item.height = widget.height_hint * scale_fac;
			widget_item.fitWidgetToItem(xpos, ypos);
			widget_item.alignWidgetItem(*this);
		}
		else {
			item->resolve(bwPoint(xpos, ypos), item_margin, scale_fac);
		}

		if (flow_direction == FLOW_DIRECTION_VERTICAL) {
			if (item->needsMarginBeforeNext(*this)) {
				ypos -= item_margin;
				height += item_margin;
			}
			else if (item->getNext(*this)) {
				ypos += 1;
				height -= 1;
			}
			ypos -= item->height;
			height += item->height;
		}
		else if (flow_direction == FLOW_DIRECTION_HORIZONTAL) {
			if (item->needsMarginBeforeNext(*this)) {
				xpos += item_margin;
			}
			else if (item->getNext(*this)) {
				xpos -= 1;
				additional_remainder_x += 1;
			}
			xpos += item->width;
			height = std::max(height, item->height);
		}
	}
	// xpos should match right side of layout precisely now.
	assert((flow_direction != FLOW_DIRECTION_HORIZONTAL) || (xpos == layout_pos.x + width));
}

bool LayoutItem::needsMarginBeforeNext(const LayoutItem& parent) const
{
	const LayoutItem* next = getNext(parent);

	if (!next) {
		return false;
	}
	else if (parent.align && (type == LAYOUT_ITEM_TYPE_WIDGET) && (next->type == LAYOUT_ITEM_TYPE_WIDGET)) {
		const WidgetLayoutItem& widget_item = static_cast<const WidgetLayoutItem&>(*this);
		const WidgetLayoutItem& next_widget_item = static_cast<const WidgetLayoutItem&>(*next);

		assert(*std::next(iterator_item) == next);
		if (widget_item.canAlignWidgetItem() && next_widget_item.canAlignWidgetItem()) {
			return false;
		}
	}

	return true;
}

unsigned int LayoutItem::countRowColumns() const
{
	unsigned int count_child_cols = 0;

	assert(type == LAYOUT_ITEM_TYPE_ROW);

	for (LayoutItem* item : child_items) {
		switch (item->type) {
			case LAYOUT_ITEM_TYPE_WIDGET:
			case LAYOUT_ITEM_TYPE_COLUMN:
				count_child_cols++;
				break;
			case LAYOUT_ITEM_TYPE_ROW:
				count_child_cols += item->countRowColumns();
				break;
			default:
				assert(0);
		}
	}

	return count_child_cols;
}

/**
 * Calculate how many margins need to be added in between child-items of this layout.
 */
unsigned int LayoutItem::countNeededMargins() const
{
	unsigned int tot_margins = 0;

	for (LayoutItem* item : child_items) {
		if (item->needsMarginBeforeNext(*this)) {
			tot_margins++;
		}
	}

	return tot_margins;
}

LayoutItem* LayoutItem::getPrevious(const LayoutItem& parent) const
{
	if (iterator_item == parent.child_items.begin()) {
		return nullptr;
	}
	else {
		IteratorItem iterator_item_prev = std::prev(iterator_item);
		return *iterator_item_prev;
	}
}
LayoutItem* LayoutItem::getNext(const LayoutItem& parent) const
{
	if (iterator_item == parent.child_items.end()) {
		return nullptr;
	}
	else {
		const IteratorItem iterator_item_next = std::next(iterator_item);
		return (iterator_item_next == parent.child_items.end()) ? nullptr : *iterator_item_next;
	}
}



/**
 * \param max_size: If \a direction is #FLOW_DIRECTION_VERTICAL, this defines the max-width of the layout.
 */
RootLayout::RootLayout(
//        FlowDirection direction,
        const int ymax,
        const unsigned int max_size,
        const bool align) :
    LayoutItem(LAYOUT_ITEM_TYPE_ROOT, align, nullptr, FLOW_DIRECTION_VERTICAL),
    max_size(max_size), ymax(ymax)
{
	
}

void RootLayout::resolve(
        const float vertical_scroll,
        const float scale_fac)
{
	// Could check if layout actually needs to be updated.

	width = max_size - (padding * 2);
	LayoutItem::resolve(bwPoint(padding, ymax - padding - vertical_scroll), item_margin, scale_fac);
}

void RootLayout::setMaxSize(const unsigned int _max_size)
{
	max_size = _max_size;
}

void RootLayout::setYmax(const int value)
{
	ymax = value;
}


ColumnLayout::ColumnLayout(
        const bool align,
        LayoutItem *parent) :
    LayoutItem(LAYOUT_ITEM_TYPE_COLUMN, align, parent, FLOW_DIRECTION_VERTICAL)
{
	
}


RowLayout::RowLayout(
        const bool align,
        LayoutItem *parent) :
    LayoutItem(LAYOUT_ITEM_TYPE_ROW, align, parent, FLOW_DIRECTION_HORIZONTAL)
{
	
}


WidgetLayoutItem::WidgetLayoutItem(
        bwWidget& widget,
        LayoutItem* parent) :
    LayoutItem(LAYOUT_ITEM_TYPE_WIDGET, false, parent), widget(widget)
{
	
}

void WidgetLayoutItem::fitWidgetToItem(
        const int xmin, const int ymax) const
{
	widget.rectangle.xmin = xmin;
	widget.rectangle.xmax = widget.rectangle.xmin + width;
	widget.rectangle.ymax = ymax;
	widget.rectangle.ymin = widget.rectangle.ymax - height;
}

void WidgetLayoutItem::alignmentSanityCheck(const LayoutItem& parent) const
{
	assert(*iterator_item != nullptr);
	assert(parent.hasChild(*this));
	assert(static_cast<const WidgetLayoutItem*>(this)->canAlignWidgetItem());

	(void)parent;
}

bool WidgetLayoutItem::canAlignWidgetItem() const
{
	return (widget.type == bwWidget::WIDGET_TYPE_PUSH_BUTTON) ||
	       (widget.type == bwWidget::WIDGET_TYPE_RADIO_BUTTON) ||
	       (widget.type == bwWidget::WIDGET_TYPE_TEXT_BOX);
}

bool WidgetLayoutItem::isAlignedtoPrevious(const LayoutItem& parent) const
{
	LayoutItem* item_prev;

	alignmentSanityCheck(parent);

	if (!parent.align) {
		return false;
	}
	if (!(item_prev = getPrevious(parent)) ||
	    (item_prev->type != LAYOUT_ITEM_TYPE_WIDGET) ||
	    (!(static_cast<WidgetLayoutItem*>(item_prev)->canAlignWidgetItem())))
	{
		return false;
	}

	return true;
}
bool WidgetLayoutItem::isAlignedtoNext(const LayoutItem& parent) const
{
	LayoutItem* item_next;

	alignmentSanityCheck(parent);

	if (!parent.align) {
		return false;
	}
	if (!(item_next = getNext(parent)) || (item_next->type != LAYOUT_ITEM_TYPE_WIDGET)) {
		return false;
	}
	else if ((item_next->type == LAYOUT_ITEM_TYPE_WIDGET) &&
	         (!(static_cast<WidgetLayoutItem*>(item_next)->canAlignWidgetItem())))
	{
		return false;
	}

	return true;
}

void WidgetLayoutItem::alignWidgetItem(const LayoutItem& parent)
{
	bwAbstractButton* abstract_button;

	if (!canAlignWidgetItem() || !(abstract_button = widget_cast<bwAbstractButton*>(&widget))) {
		return;
	}
	abstract_button->rounded_corners = 0;

	if (!isAlignedtoPrevious(parent)) {
		abstract_button->rounded_corners |= (parent.flow_direction == FLOW_DIRECTION_HORIZONTAL) ?
		                                        (TOP_LEFT | BOTTOM_LEFT) : (TOP_LEFT | TOP_RIGHT);
	}
	if (!isAlignedtoNext(parent)) {
		abstract_button->rounded_corners |= (parent.flow_direction == FLOW_DIRECTION_HORIZONTAL) ?
		                                        (TOP_RIGHT| BOTTOM_RIGHT) : (BOTTOM_LEFT | BOTTOM_RIGHT);
	}
}
