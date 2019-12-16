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
#include <memory>

#include "bwEvent.h"
#include "bwScrollBar.h"
#include "bwStyle.h"
#include "screen_graph/Node.h"
#include "screen_graph/ScreenGraph.h"

#include "Font.h"

namespace bWidgetsDemo {

class Stage {
  friend class ScrollbarApplyValueFunctor;
  friend class StyleSetter;
  friend class UseCSSVersionToggleSetter;
  friend class UseFontSubPixelsToggleSetter;

 public:
  Stage(const unsigned int mask_width, const unsigned int mask_height);
  virtual ~Stage() = default;

  void draw();

  void handleMouseMovementEvent(const class MouseEvent& event);
  void handleMouseButtonEvent(const class MouseEvent& event);
  void handleMouseScrollEvent(const class MouseEvent& event);
  void handleWindowResizeEvent(const class Window& win);

  static void setInterfaceScale(const float value);
  static void setFontTightPositioning(const bool value);
  static void setFontAntiAliasingMode(const Font::AntiAliasingMode aa_mode);
  static void setFontHinting(const bool value);
  static void setFontSubPixelPositioning(const bool value);

 protected:
  bWidgets::bwScreenGraph::ScreenGraph screen_graph;

  // Static members, global UI data for all stages
  static bWidgets::bwPtr<bWidgets::bwStyle> style;
  static bWidgets::bwPtr<class Font> font;
  static bWidgets::bwPtr<class IconMap> icon_map;
  static bWidgets::bwPtr<class StyleSheet> style_sheet;
  static float interface_scale;

  unsigned int mask_width, mask_height;
  int vert_scroll = 0;

  class RootLayout& Layout() const;

  virtual void activateStyleID(bWidgets::bwStyle::TypeID type_id);

 private:
  // Not part of the layout yet. We'd need to support horizontal root layouts first.
  bWidgets::bwScreenGraph::WidgetNode scrollbar_node;

  static void StyleSheetPolish(bWidgets::bwWidget& widget);

  void initFonts();
  void initIcons();
  void setStyleSheet(const std::string& filepath);
  void drawScrollbars();
  void updateContentBounds();
  void validizeScrollValue();

  bool shouldHaveScrollbars() const;
  unsigned int getScrollbarWidth() const;
  unsigned int getContentWidth() const;
  unsigned int getContentHeight() const;
  void setScrollValue(int value);
};

}  // namespace bWidgetsDemo
