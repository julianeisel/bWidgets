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

#include <string>
#include <array>

namespace bWidgetsDemo {

typedef enum ShaderTypeID {
	SHADER_TYPE_VERTEX,
	SHADER_TYPE_FRAGMENT,

	SHADER_TYPE_TOT
} ShaderTypeID;

class ShaderProgram
{
public:
	typedef enum {
		ID_UNIFORM_COLOR,
		ID_SMOOTH_COLOR,
		ID_BITMAP_TEXTURE_UNIFORM_COLOR,

		SHADER_PROGRAM_ID_TOT
	} ShaderProgramID;

	ShaderProgram(ShaderProgramID shader_program_id);
	~ShaderProgram();

	unsigned int ProgramID() const;

	ShaderInterface* getInterface() const;

private:
	std::array<unsigned int, SHADER_TYPE_TOT> shader_ids;
	unsigned int programID;
	ShaderInterface* interface;
};

} // namespace bWidgetsDemo
