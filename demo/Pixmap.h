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

#include <vector>


namespace bWidgetsDemo {

class Pixmap
{
public:
	Pixmap(
	        const int width, const int height,
	        const unsigned int num_channels,
	        const unsigned int bits_per_channel = 8,
	        const unsigned int row_padding = 0);

	void fill(const unsigned char* bytes);

	std::vector<unsigned char>& getBytes();
	const std::vector<unsigned char>& getBytes() const;
	int width() const;
	int height() const;
	unsigned int getBitDepth() const;
	unsigned int getNumChannels() const;

private:
	std::vector<unsigned char> _bytes;
	int _width, _height;
	unsigned int _num_channels;
	unsigned int _bits_per_channel;
	unsigned int _row_padding;
};

} // namespace bWidgetsDemo
