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

#include "bwPaintEngine.h"

namespace bWidgetsDemo {

class GawainPaintEngine : public bWidgets::bwPaintEngine {
 public:
  GawainPaintEngine(class Font&, class IconMap&);

  void setupViewport(const bWidgets::bwRectanglePixel&, const class bWidgets::bwColor&) override;

  void drawPolygon(const class bWidgets::bwPainter&, const class bWidgets::bwPolygon&) override;
  void drawText(const class bWidgets::bwPainter&,
                const std::string&,
                const bWidgets::bwRectanglePixel&,
                const bWidgets::TextAlignment) override;
  void drawIcon(const class bWidgets::bwPainter&,
                const bWidgets::bwIconInterface&,
                const bWidgets::bwRectanglePixel&) override;

 private:
  class Font& font;
  class IconMap& icon_map;
};

}  // namespace bWidgetsDemo
