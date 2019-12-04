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

#include <cassert>
#include <iostream>

#include "bwAbstractButton.h"
#include "bwPainter.h"
#include "bwPanel.h"

#include "Layout.h"

using namespace bWidgetsDemo;
using namespace bWidgets;  // Less verbose

namespace bWidgetsDemo {

void resolveScreenGraphNodeLayout(bwScreenGraph::Node& node,
                                  const float vertical_scroll,
                                  const float scale_fac)
{
  if (LayoutItem* layout = static_cast<LayoutItem*>(node.Layout())) {
    if (RootLayout* root = dynamic_cast<RootLayout*>(layout)) {
      assert(node.Children());
      root->resolve(*node.Children(), vertical_scroll, scale_fac);
    }
  }
}

}  // namespace bWidgetsDemo

LayoutItem::LayoutItem(LayoutItemType item_type, const bool align, FlowDirection flow_direction)
    : type(item_type), flow_direction(flow_direction), align(align)
{
}

unsigned int LayoutItem::getHeight() const
{
  return height;
}

static bwScreenGraph::Node* getNextUnhiddenNode(
    const bwScreenGraph::Node::ChildList::const_iterator& current)
{
  for (auto iter = ++bwScreenGraph::Node::ChildList::const_iterator(current);
       iter != (*current)->Parent()->Children()->end();
       ++iter) {
    if (bwWidget* widget = (*iter)->Widget()) {
      if (!widget->hidden) {
        return iter->get();
      }
    }
    else {
      return iter->get();
    }
  }

  return nullptr;
}
static bwScreenGraph::Node* getPreviousUnhiddenNode(
    const bwScreenGraph::Node::ChildIterator& current)
{
  for (auto iter = bwScreenGraph::Node::ChildList::reverse_iterator(current);
       iter != (*current)->Parent()->Children()->rend();
       ++iter) {
    if (bwWidget* widget = (*iter)->Widget()) {
      if (!widget->hidden) {
        return iter->get();
      }
    }
    else {
      return iter->get();
    }
  }

  return nullptr;
}

bool shouldWidgetAlignToPrevious(const bwScreenGraph::Node::ChildIterator& current)
{
  bwScreenGraph::Node* prev_item = getPreviousUnhiddenNode(current);
  bwWidget* prev_widget = prev_item ? prev_item->Widget() : nullptr;

  if (!prev_item || !prev_widget || !prev_widget->canAlign()) {
    return false;
  }

  return true;
}
bool shouldWidgetAlignToNext(const bwScreenGraph::Node::ChildIterator& current)
{
  bwScreenGraph::Node* next_item = getNextUnhiddenNode(current);
  bwWidget* next_widget = next_item ? next_item->Widget() : nullptr;

  if (!next_item || !next_widget || !next_widget->canAlign()) {
    return false;
  }

  return true;
}
static void alignNode(bwScreenGraph::Node::ChildIterator node_iter,
                      const LayoutItem::FlowDirection flow_direction)
{
  bwWidget* widget = (*node_iter)->Widget();
  bwAbstractButton* abstract_button;

  if (!widget || !widget->canAlign() ||
      !(abstract_button = widget_cast<bwAbstractButton*>(widget))) {
    return;
  }
  abstract_button->rounded_corners = 0;

  if (!shouldWidgetAlignToPrevious(node_iter)) {
    abstract_button->rounded_corners |= (flow_direction == LayoutItem::FLOW_DIRECTION_HORIZONTAL) ?
                                            (TOP_LEFT | BOTTOM_LEFT) :
                                            (TOP_LEFT | TOP_RIGHT);
  }
  if (!shouldWidgetAlignToNext(node_iter)) {
    abstract_button->rounded_corners |= (flow_direction == LayoutItem::FLOW_DIRECTION_HORIZONTAL) ?
                                            (TOP_RIGHT | BOTTOM_RIGHT) :
                                            (BOTTOM_LEFT | BOTTOM_RIGHT);
  }
}

static bool needsMarginAfterNode(const bwScreenGraph::Node::ChildList::const_iterator node_iter,
                                 const bool align)
{
  const bwWidget* widget = (*node_iter)->Widget();
  const bwScreenGraph::Node* next_iter = getNextUnhiddenNode(node_iter);
  const bwWidget* next_widget = next_iter ? next_iter->Widget() : nullptr;

  if (!next_iter) {
    return false;
  }

  if (align && widget && next_widget) {
    if (widget->canAlign() && next_widget->canAlign()) {
      return false;
    }
  }

  return true;
}

namespace bWidgetsDemo {
int getNodeWidth(const bwScreenGraph::Node& node)
{
  if (const LayoutItem* layout = static_cast<LayoutItem*>(node.Layout())) {
    return layout->width;
  }
  if (const bwWidget* widget = node.Widget()) {
    return widget->rectangle.width();
  }

  return 0;
}
int getNodeHeight(const bwScreenGraph::Node& node)
{
  if (const LayoutItem* layout = static_cast<LayoutItem*>(node.Layout())) {
    return layout->height;
  }
  if (const bwWidget* widget = node.Widget()) {
    return widget->rectangle.height();
  }

  return 0;
}
}  // namespace bWidgetsDemo

void LayoutItem::resolvePanelContents(bwScreenGraph::Node& panel_node,
                                      const bwPoint& panel_pos,
                                      const unsigned int padding,
                                      const unsigned int item_margin,
                                      const float scale_fac)
{
  const bwPanel* panel = static_cast<bwPanel*>(panel_node.Widget());
  LayoutItem* layout = static_cast<LayoutItem*>(panel_node.Layout());
  const int initial_width = layout->width;
  bwPoint panel_items_pos = panel_pos;

  panel_items_pos.x += padding;
  panel_items_pos.y -= padding + panel->header_height + padding;

  layout->width -= 2.0f * padding;

  layout->resolve(panel_node.Children(), panel_items_pos, item_margin, scale_fac);

  layout->width = initial_width;
}

void LayoutItem::resolve(bwScreenGraph::Node::ChildList* children,
                         const bwPoint& layout_pos,
                         const unsigned int item_margin,
                         const float scale_fac)
{
  int xpos = layout_pos.x;
  int ypos = layout_pos.y;
  int item_width_base = 0;
  // Width calculations may have imprecisions so widths don't add up to full layout width. We 'fix'
  // this by adding a pixel to each layout-item until the remainder is 0, meaning total width
  // matches parent width precisely. Also makes layout less jaggy on window size changes.
  int additional_remainder_x = 0;

  location = layout_pos;

  if (children == nullptr) {
    return;
  }

  height = 0;

  if (flow_direction == FLOW_DIRECTION_HORIZONTAL) {
    const unsigned int margin_count = countNeededMargins(*children);
    const unsigned int width_no_margins = width - (margin_count * item_margin);
    // The max-width of each item is the max-width of the parent, divided by the count of its
    // horizontally placed sub-items. In other words, each item has the same max-width that
    // add up to the parent's max-width. From this max-width, the item margins are subtracted.
    unsigned int tot_row_cols = countRowColumns(*children);

    if (tot_row_cols == 0) {
      width = 0;
    }
    else {
      item_width_base = width_no_margins / tot_row_cols;
      additional_remainder_x = width_no_margins % tot_row_cols;
    }
    assert(additional_remainder_x >= 0);
  }
  else {
    item_width_base = width;
  }

  for (auto node_iter = children->begin(); node_iter != children->end(); ++node_iter) {
    bwScreenGraph::Node& node = **node_iter;
    LayoutItem* layout = static_cast<LayoutItem*>(node.Layout());
    bwWidget* widget = node.Widget();
    int item_width = item_width_base;

    if (widget && widget->hidden) {
      continue;
    }

    const bwScreenGraph::Node* next = getNextUnhiddenNode(node_iter);

    // Simple correction for precision issues.
    if (additional_remainder_x > 0) {
      if (next) {
        item_width++;
        additional_remainder_x--;
      }
      else {
        // For the last button in a row, additional_remainder_x may be
        // larger than 1, so add all that is left to it.
        //				assert(additional_remainder_x <= 2);
        item_width += additional_remainder_x;
        additional_remainder_x = 0;
      }
    }

    if (widget && widget->type == bwWidget::WIDGET_TYPE_PANEL) {
      bwPanel& panel = static_cast<bwPanel&>(*widget);

      assert(layout);
      layout->width = item_width;
      layout->height = 0;

      panel.header_height = panel.getHeaderHeightHint() * scale_fac;
      if (panel.panel_state == bwPanel::PANEL_OPEN) {
        resolvePanelContents(node, bwPoint(xpos, ypos), item_margin, item_margin, scale_fac);
        layout->height += 3 * item_margin;
      }
      layout->height += panel.header_height;
      panel.rectangle.set(xpos, layout->width, ypos - layout->height, layout->height);
      location.y = widget->rectangle.ymin;
    }
    else if (layout) {
      layout->width = item_width;
      layout->resolve(node.Children(), bwPoint(xpos, ypos), item_margin, scale_fac);
      location.y = ypos;
    }
    if (widget) {
      const int widget_height = layout ? layout->height : widget->height_hint * scale_fac;
      widget->rectangle.set(xpos, item_width, ypos - widget_height, widget_height);
      location.y = widget->rectangle.ymin;
      if (align) {
        alignNode(node_iter, flow_direction);
      }
    }

    const int child_width = getNodeWidth(node);
    const int child_height = getNodeHeight(node);

    if (flow_direction == FLOW_DIRECTION_VERTICAL) {
      if (needsMarginAfterNode(node_iter, align)) {
        ypos -= item_margin;
        height += item_margin;
      }
      else if (next) {
        ypos += 1;
        height -= 1;
      }
      ypos -= child_height;
      height += child_height;
    }
    else if (flow_direction == FLOW_DIRECTION_HORIZONTAL) {
      if (needsMarginAfterNode(node_iter, align)) {
        xpos += item_margin;
      }
      else if (next) {
        xpos -= 1;
        additional_remainder_x += 1;
      }
      xpos += child_width;
      height = std::max(height, child_height);
    }
  }
  // xpos should match right side of layout precisely now.
  assert((flow_direction != FLOW_DIRECTION_HORIZONTAL) || (xpos == layout_pos.x + width));
}

bwRectanglePixel LayoutItem::getRectangle()
{
  return bwRectanglePixel(location.x, location.x + width, location.y, location.y + height);
}

unsigned int LayoutItem::countRowColumns(const bwScreenGraph::Node::ChildList& children) const
{
  unsigned int count_child_cols = 0;

  assert(type == LAYOUT_ITEM_TYPE_ROW);

  for (auto& node : children) {
    const bwWidget* widget = node->Widget();
    const LayoutItem* layout = static_cast<LayoutItem*>(node->Layout());

    if (widget && widget->hidden) {
      continue;
    }
    if (widget) {
      count_child_cols++;
    }
    else if (layout && layout->type != LAYOUT_ITEM_TYPE_ROW) {
      count_child_cols++;
    }
    else if (layout) {
      count_child_cols += layout->countRowColumns(*node->Children());
    }
  }

  return count_child_cols;
}

/**
 * Calculate how many margins need to be added in between child-items of this layout.
 */
unsigned int LayoutItem::countNeededMargins(const bwScreenGraph::Node::ChildList& children) const
{
  unsigned int tot_margins = 0;

  for (auto child_iter = children.begin(); child_iter != children.end(); ++child_iter) {
    const bwScreenGraph::Node& child = **child_iter;
    const bwWidget* widget = child.Widget();

    if (widget && widget->hidden) {
      continue;
    }
    if (needsMarginAfterNode(child_iter, align)) {
      tot_margins++;
    }
  }

  return tot_margins;
}

/**
 * \param max_size: If \a direction is #FLOW_DIRECTION_VERTICAL, this defines the max-width of the
 * layout.
 */
RootLayout::RootLayout(
    //        FlowDirection direction,
    const int ymax,
    const unsigned int max_size,
    const bool align)
    : LayoutItem(LAYOUT_ITEM_TYPE_ROOT, align, FLOW_DIRECTION_VERTICAL),
      max_size(max_size),
      ymax(ymax)
{
}

void RootLayout::resolve(bwScreenGraph::Node::ChildList& children,
                         const float vertical_scroll,
                         const float scale_fac)
{
  // Could check if layout actually needs to be updated.

  bwPoint layout_position{(float)padding, ymax - padding - vertical_scroll};

  width = max_size - (padding * 2);
  location = layout_position;
  LayoutItem::resolve(&children, layout_position, item_margin, scale_fac);
}

void RootLayout::setMaxSize(const unsigned int _max_size)
{
  max_size = _max_size;
}

void RootLayout::setYmax(const int value)
{
  ymax = value;
}

ColumnLayout::ColumnLayout(const bool align)
    : LayoutItem(LAYOUT_ITEM_TYPE_COLUMN, align, FLOW_DIRECTION_VERTICAL)
{
}

RowLayout::RowLayout(const bool align)
    : LayoutItem(LAYOUT_ITEM_TYPE_ROW, align, FLOW_DIRECTION_HORIZONTAL)
{
}

PanelLayout::PanelLayout() : LayoutItem(LAYOUT_ITEM_TYPE_PANEL, false, FLOW_DIRECTION_VERTICAL)
{
}
