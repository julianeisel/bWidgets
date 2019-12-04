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

#include "File.h"

using namespace bWidgetsDemo;

File::File(const std::string& path, std::ios::openmode mode)
    : _path(path), _file_stream(path, mode)
{
  assert(_file_stream.is_open());
}

std::string File::readIntoString()
{
  std::string string;
  std::string line;

  assert(_file_stream.is_open());
  while (getline(_file_stream, line)) {
    string += line + '\n';
  }

  return string;
}

bool File::readBytes(char* bytes, const unsigned int num_bytes, bool reset_cursor)
{
  assert(_file_stream.is_open());
  _file_stream.read(bytes, num_bytes);
  if (reset_cursor) {
    _file_stream.seekg(0);
  }
  return _file_stream.good();
}

namespace bWidgetsDemo {
std::ostream& operator<<(std::ostream& stream, const File& file)
{
  stream << file._path;
  return stream;
}
}  // namespace bWidgetsDemo
