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

#include <iostream>
#include <array>

#include "Pixmap.h"

#include "bwIconInterface.h"
#include "bwUtil.h"

namespace bWidgetsDemo {

struct IconReadData;

class Icon : public bWidgets::bwIconInterface {
 public:
  Icon(const unsigned int size,
       const unsigned int num_channels,
       const unsigned int bits_per_channel);

  bool isValid() const override;

  Pixmap &getPixmap();  // TODO should get rid of this.
  const Pixmap &getPixmap() const;

 private:
  Pixmap _pixmap;
};

class IconMap {
  friend class IconMapReader;

 public:
  Icon &getIcon(unsigned int index);
  ~IconMap() = default;

 private:
  IconMap() = default;

  std::array<bWidgets::bwPtr<Icon>, 30 * 26> icons;  // ICON_GRID_ROWS * ICON_GRID_COLS
};

class IconMapReader {
 public:
  IconMapReader();
  ~IconMapReader();

  bWidgets::bwPtr<IconMap> readIconMapFromPNGFile(class File &);

 private:
  bWidgets::bwPtr<IconReadData> read_data;
};

}  // namespace bWidgetsDemo
