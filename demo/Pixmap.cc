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

#include <algorithm>
#include <cassert>

#include "Pixmap.h"

using namespace bWidgetsDemo;

static unsigned int get_num_row_bytes_impl(int width,
                                           unsigned int num_channels,
                                           unsigned int bits_per_channel,
                                           unsigned int row_padding)
{
  assert((bits_per_channel % 8) == 0);
  return width * (bits_per_channel / 8) * num_channels + row_padding;
}

Pixmap::Pixmap(const int width,
               const int height,
               const unsigned int num_channels,
               const unsigned int bits_per_channel,
               const unsigned int row_padding)
    : _bytes(height * get_num_row_bytes_impl(width, num_channels, bits_per_channel, row_padding)),
      _width(width),
      _height(height),
      _num_channels(num_channels),
      _bits_per_channel(bits_per_channel),
      _row_padding(row_padding)
{
}

void Pixmap::fill(const unsigned char *bytes)
{
  if (_bytes.size() > 0) {
    std::copy_n(bytes, _bytes.size(), &_bytes[0]);
  }
}

std::vector<unsigned char> &Pixmap::getBytes()
{
  return _bytes;
}
const std::vector<unsigned char> &Pixmap::getBytes() const
{
  return _bytes;
}

int Pixmap::width() const
{
  return _width;
}
int Pixmap::height() const
{
  return _height;
}

unsigned int Pixmap::getBitDepth() const
{
  return _bits_per_channel;
}

unsigned int Pixmap::getNumChannels() const
{
  return _num_channels;
}
