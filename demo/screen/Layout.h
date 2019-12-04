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

#include "screen_graph/Node.h"

namespace bWidgets {
class bwPanel;
class bwPoint;
class bwStyle;
class bwWidget;
}  // namespace bWidgets

namespace bWidgetsDemo {

void resolveScreenGraphNodeLayout(bWidgets::bwScreenGraph::Node &node,
                                  const float vertical_scroll,
                                  const float scale_fac);

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
class LayoutItem : public bWidgets::bwLayoutInterface {
  friend int getNodeWidth(const bWidgets::bwScreenGraph::Node &);
  friend int getNodeHeight(const bWidgets::bwScreenGraph::Node &);

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

  virtual void resolve(bWidgets::bwScreenGraph::Node::ChildList *chilren,
                       const bWidgets::bwPoint &layout_pos,
                       const unsigned int item_margin,
                       const float scale_fac);

  unsigned int getHeight() const;

  const LayoutItemType type;
  const FlowDirection flow_direction;
  const bool align;

 protected:
  // Protected constructor to force calling through inherited class (pseudo abstract).
  LayoutItem(LayoutItemType layout_type,
             const bool align,
             FlowDirection flow_direction = FLOW_DIRECTION_HORIZONTAL);

  using LayoutItemList = std::list<bWidgets::bwPtr<LayoutItem>>;
  using IteratorItem = LayoutItemList::const_iterator;

  int width{0}, height{0};

  static void resolvePanelContents(bWidgets::bwScreenGraph::Node &panel_node,
                                   const bWidgets::bwPoint &panel_pos,
                                   const unsigned int padding,
                                   const unsigned int item_margin,
                                   const float scale_fac);

 private:
  unsigned int countRowColumns(const bWidgets::bwScreenGraph::Node::ChildList &children) const;
  unsigned int countNeededMargins(const bWidgets::bwScreenGraph::Node::ChildList &children) const;
};

/**
 * \brief The layout item at the root of the layout-item tree.
 *
 * A RootLayout can be used to manage child-layouts more easily.
 *
 * \note Only root-layouts with \a flow_direction FLOW_DIRECTION_VERTICAL are supported right now.
 */
class RootLayout : public LayoutItem {
 public:
  RootLayout(
      //	        FlowDirection direction,
      const int ymax,
      const unsigned int max_size,
      const bool align = false);

  void resolve(bWidgets::bwScreenGraph::Node::ChildList &children,
               const float vertical_scroll,
               const float scale_fac);
  void setMaxSize(const unsigned int max_size);
  void setYmax(const int value);

  unsigned int padding = 0;
  unsigned int item_margin = 0;

 private:
  unsigned int max_size;
  int ymax;
};

class ColumnLayout : public LayoutItem {
 public:
  explicit ColumnLayout(const bool align = false);
};

class RowLayout : public LayoutItem {
 public:
  explicit RowLayout(const bool align = false);
};

class PanelLayout : public LayoutItem {
 public:
  explicit PanelLayout();
};

}  // namespace bWidgetsDemo
