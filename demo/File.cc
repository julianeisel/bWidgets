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

#include <assert.h>
#include <fstream>

#include "File.h"

using namespace bWidgetsDemo;



File::File(const std::string& path) :
    path(path)
{
	
}

std::string File::readIntoString()
{
	std::ifstream file_stream(path, std::ios::in);
	std::string string = "";

	if (file_stream.is_open()) {
		std::string line = "";
		while (getline(file_stream, line)) {
			string += line + '\n';
		}
		file_stream.close();
	}
	else {
		assert(0);
	}

	return string;
}
