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

#include "bwUtil.h"


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
		ID_SUBPIXEL_BITMAP_TEXTURE_UNIFORM_COLOR,
		ID_TEXTURE_RECT,

		SHADER_PROGRAM_ID_TOT
	} ShaderProgramID;
	using ShaderIDArray = std::array<unsigned int, SHADER_TYPE_TOT>;

	static ShaderProgram& getShaderProgram(ShaderProgramID shader_program_id);

	unsigned int ProgramID() const;
	const ShaderInterface& getInterface() const;

private:
	ShaderProgram(ShaderProgramID shader_program_id);
	~ShaderProgram();
	friend bWidgets::bwPtr<ShaderProgram>::deleter_type;

	using ShaderProgramCache = std::array<bWidgets::bwPtr<ShaderProgram>, ShaderProgram::SHADER_PROGRAM_ID_TOT>;
	static ShaderProgramCache cache;

	ShaderIDArray shader_ids = {};
	unsigned int programID = 0;
	ShaderInterface* interface;
};

} // namespace bWidgetsDemo
