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
#include "bwStyle.h"
#include "screen_graph/Node.h"
#include "screen_graph/ScreenGraph.h"

#include "RNAProperty.h"

#include "Font.h"

namespace bWidgetsDemo {

class MouseEvent;
class Window;

class Stage {
  friend class UseFontSubPixelsToggleSetter;

 public:
  Stage(const unsigned int mask_width, const unsigned int mask_height);
  virtual ~Stage();

  void draw();

  void handleMouseMovementEvent(const MouseEvent& event);
  void handleMouseButtonEvent(const MouseEvent& event);
  void handleMouseScrollEvent(const MouseEvent& event,
                              enum bWidgets::bwMouseWheelEvent::Direction dir);
  void handleWindowResizeEvent(const Window& win);

  void setContentScale(float scale_x, float scale_y);
  static void setInterfaceScale(const float value);
  static void setFontSize(const float size);
  static void setFontTightPositioning(const bool value);
  static void setFontAntiAliasingMode(const Font::AntiAliasingMode aa_mode);
  static void setFontHinting(const bool value);
  static void setFontSubPixelPositioning(const bool value);

 protected:
  virtual void activateStyleID(bWidgets::bwStyle::TypeID type_id);

  bWidgets::bwScreenGraph::ScreenGraph screen_graph;

  // Static members, global UI data for all stages
  static std::unique_ptr<bWidgets::bwStyle> style;
  static std::unique_ptr<class Font> font;
  static std::unique_ptr<class IconMap> icon_map;
  static std::unique_ptr<class StyleSheet> style_sheet;
  static float interface_scale;

  unsigned int mask_width, mask_height;

 private:
  static void StyleSheetPolish(bWidgets::bwWidget& widget);

  void initFonts();
  void initIcons();
  void setStyleSheet(const std::string& filepath);
};

}  // namespace bWidgetsDemo
