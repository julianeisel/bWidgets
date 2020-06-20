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
 * Original work Copyright (c) 2020 Julian Eisel
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include <iostream>

extern "C" {
#include "gawain/gwn_immediate.h"
#include "gawain/gwn_shader_interface.h"
}

#include "File.h"

#include "GPUShader.h"

namespace bWidgetsDemo {

using ShaderIDFileNames = std::array<const char*, 2>;
static ShaderIDFileNames shader_id_file_map[GPUShader::SHADER_ID_TOT] = {
    {"uniform_color_vert.glsl", "uniform_color_frag.glsl"},
    {"smooth_color_vert.glsl", "smooth_color_frag.glsl"},
    {"texture_vert.glsl", "pixel_alpha_mask_texture_uniform_color_frag.glsl"},
    {"texture_vert.glsl", "subpixel_alpha_mask_texture_uniform_color_frag.glsl"},
    {"texture_vert.glsl", "texture_frag.glsl"},
};

using CacheType = std::array<std::unique_ptr<GPUShader>, GPUShader::SHADER_ID_TOT>;
static CacheType CACHE;

GPUShader::GPUShader(const std::string& vertexcode, const std::string& fragcode)
{
  GLint status;
  GLchar log[5000];
  GLsizei length = 0;

  m_vertex = glCreateShader(GL_VERTEX_SHADER);
  m_fragment = glCreateShader(GL_FRAGMENT_SHADER);
  m_program = glCreateProgram();

  if (!m_vertex || !m_fragment || !m_program) {
    std::cerr << "Error creating shaders or program." << std::endl;
    throw std::exception();
  }

  /* Vertex shader */

  const char* vertexcode_c_str[1] = {vertexcode.c_str()};
  glAttachShader(m_program, m_vertex);
  glShaderSource(m_vertex, 1, vertexcode_c_str, NULL);
  glCompileShader(m_vertex);
  glGetShaderiv(m_vertex, GL_COMPILE_STATUS, &status);

  if (!status) {
    glGetShaderInfoLog(m_vertex, sizeof(log), &length, log);
    std::cerr << "Error compiling vertex shader:\n" << log << std::endl;
    throw std::exception();
  }

  /* Fragment shader */

  const char* fragcode_c_str[1] = {fragcode.c_str()};
  glAttachShader(m_program, m_fragment);
  glShaderSource(m_fragment, 1, fragcode_c_str, NULL);
  glCompileShader(m_fragment);
  glGetShaderiv(m_fragment, GL_COMPILE_STATUS, &status);

  if (!status) {
    glGetShaderInfoLog(m_fragment, sizeof(log), &length, log);
    std::cerr << "Error compiling fragment shader:\n" << log << std::endl;
    throw std::exception();
  }

  glLinkProgram(m_program);
  glGetProgramiv(m_program, GL_LINK_STATUS, &status);
  if (!status) {
    glGetProgramInfoLog(m_program, sizeof(log), &length, log);
    std::cerr << "Error linking shader program:\n" << log << std::endl;
    throw std::exception();
  }

  m_interface = GWN_shaderinterface_create(m_program);
  glDetachShader(m_program, m_vertex);
  glDetachShader(m_program, m_fragment);
  glDeleteShader(m_vertex);
  glDeleteShader(m_fragment);
}

GPUShader::~GPUShader()
{
  if (m_vertex) {
    glDeleteShader(m_vertex);
  }
  if (m_fragment) {
    glDeleteShader(m_fragment);
  }
  if (m_program) {
    glDeleteProgram(m_program);
  }

  if (m_interface) {
    GWN_shaderinterface_discard(m_interface);
  }
}

void GPUShader::clearCache()
{
  for (auto& shader : CACHE) {
    shader = nullptr;
  }
}

void GPUShader::immBind(ID id)
{
  GPUShader& shader = ensureCached(id);
  immBindProgram(shader.m_program, shader.m_interface);
}

void GPUShader::immUnbind()
{
  immUnbindProgram();
}

std::unique_ptr<GPUShader> GPUShader::createFromID(GPUShader::ID id)
{
  ShaderIDFileNames file_names = shader_id_file_map[id];
  File vert_file(std::string(SHADERS_PATH_STR) + "/" + file_names[0]);
  File frag_file(std::string(SHADERS_PATH_STR) + "/" + file_names[1]);

  return std::unique_ptr<GPUShader>(
      new GPUShader{vert_file.readIntoString(), frag_file.readIntoString()});
}

GPUShader& GPUShader::ensureCached(ID id)
{
  if (!CACHE[id]) {
    CACHE[id] = createFromID(id);
  }

  return *CACHE[id];
}

}  // namespace bWidgetsDemo
