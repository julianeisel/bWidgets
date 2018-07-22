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
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "GPU.h"
extern "C" {
#include "../extern/gawain/gawain/shader_interface.h"
}

#include "File.h"

#include "ShaderProgram.h"


namespace bWidgetsDemo {

class ShaderType
{
public:
	ShaderType(ShaderTypeID id, GLuint gl_id) :
		id(id), gl_id(gl_id)
	{

	}

	ShaderTypeID id;
	GLuint gl_id;
};

static ShaderType shader_types[SHADER_TYPE_TOT] = {
	ShaderType{SHADER_TYPE_VERTEX, GL_VERTEX_SHADER},
	ShaderType{SHADER_TYPE_FRAGMENT, GL_FRAGMENT_SHADER},
};

class ShaderProgramType
{
public:
	ShaderProgramType(const std::array<std::string, SHADER_TYPE_TOT>& names) :
		shader_names(names)
	{

	}
	std::array<std::string, SHADER_TYPE_TOT> shader_names;
};

static ShaderProgramType shader_program_types[ShaderProgram::SHADER_PROGRAM_ID_TOT] = {
	ShaderProgramType{{"uniform_color_vert.glsl", "uniform_color_frag.glsl"}},
	ShaderProgramType{{"smooth_color_vert.glsl", "smooth_color_frag.glsl"}},
	ShaderProgramType{{"texture_vert.glsl", "bitmap_texture_uniform_color_frag.glsl"}},
	ShaderProgramType{{"texture_vert.glsl", "texture_frag.glsl"}},
};

} // namespace bWidgetsDemo

using namespace bWidgetsDemo;

ShaderProgram::ShaderProgramCache ShaderProgram::cache;


static unsigned int shaderprog_compileShader(const std::string& shader_str, const ShaderType& shader_type)
{
	GLuint shader_id = glCreateShader(shader_type.gl_id);
	const char* shader_c_str = shader_str.c_str();

	glShaderSource(shader_id, 1, &shader_c_str, NULL);
	glCompileShader(shader_id);

#ifndef NDEBUG
	GLint success = GL_FALSE;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		int len;

		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &len);

		std::vector<GLchar> log(len);
		glGetShaderInfoLog(shader_id, len, &len, &log[0]);
		std::cout << "Error compiling shader: " << std::string(&log[0]) << std::endl;
		assert(false);
	}
#endif

	return shader_id;
}

static unsigned int shaderprog_linkProgram(const ShaderProgram::ShaderIDArray& shader_ids)
{
	unsigned int program_id = glCreateProgram();

	for (unsigned int shader_id : shader_ids) {
		glAttachShader(program_id, shader_id);
	}
	glLinkProgram(program_id);

#ifndef NDEBUG
	GLint success = GL_FALSE;
	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int len;

		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &len);

		std::vector<GLchar> log(len);
		glGetProgramInfoLog(program_id, len, &len, &log[0]);
		std::cout << "Error linking shaders: " << std::string(&log[0]) << std::endl;
		assert(false);
	}
#endif

	return program_id;
}

static ShaderProgram::ShaderIDArray shaderprog_compileShaders(ShaderProgramType& type)
{
	ShaderProgram::ShaderIDArray shader_ids;

	for (unsigned int i = 0; i < shader_ids.size(); i++) {
		File shader_file(std::string(SHADERS_PATH_STR) + "/" + type.shader_names[i]);
		std::string shader_str = shader_file.readIntoString();
		unsigned int shader_id = shaderprog_compileShader(shader_str, shader_types[i]);
		shader_ids[i] = shader_id;
	}

	return shader_ids;
}

ShaderProgram::ShaderProgram(ShaderProgram::ShaderProgramID shader_program_id)
{
	ShaderProgramType& type = shader_program_types[shader_program_id];

	shader_ids = shaderprog_compileShaders(type);
	programID = shaderprog_linkProgram(shader_ids);
	interface = ShaderInterface_create(programID);
}

ShaderProgram::~ShaderProgram()
{
	ShaderInterface_discard(interface);
	for (unsigned int shader_id : shader_ids) {
		glDeleteShader(shader_id);
	}
	glDeleteProgram(programID);
}

ShaderProgram& ShaderProgram::getShaderProgram(ShaderProgram::ShaderProgramID shader_program_id)
{
	if (!cache[shader_program_id]) {
		cache[shader_program_id] = bWidgets::bwPointer<ShaderProgram>(new ShaderProgram(shader_program_id));
	}

	return *cache[shader_program_id];
}

unsigned int ShaderProgram::ProgramID() const
{
	return programID;
}

const ShaderInterface& ShaderProgram::getInterface() const
{
	return *interface;
}
