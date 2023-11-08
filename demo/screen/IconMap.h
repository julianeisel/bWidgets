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

#include <array>
#include <iostream>
#include <memory>

#include "Pixmap.h"

#include "bwIconInterface.h"
#include "bwUtil.h"

namespace bWidgetsDemo {

struct IconReadData;
class File;

class Icon : public bWidgets::bwIconInterface {
 public:
  Icon(const unsigned int size,
       const unsigned int num_channels,
       const unsigned int bits_per_channel);

  auto isValid() const -> bool override;

  auto getPixmap() -> Pixmap&;  // TODO should get rid of this.
  auto getPixmap() const -> const Pixmap&;

 private:
  Pixmap _pixmap;
};

class IconMap {
  friend class IconMapReader;

 public:
  ~IconMap() = default;

  auto getIcon(unsigned int index) -> Icon&;

 private:
  IconMap() = default;

  std::array<std::unique_ptr<Icon>, 30 * 26> icons;  // ICON_GRID_ROWS * ICON_GRID_COLS
};

class IconMapReader {
 public:
  IconMapReader();
  ~IconMapReader();

  auto readIconMapFromPNGFile(File&) -> std::unique_ptr<IconMap>;

 private:
  std::unique_ptr<IconReadData> read_data;
};

}  // namespace bWidgetsDemo
