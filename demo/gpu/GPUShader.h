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

#pragma once

#include <string>
#include <array>

#include "bwUtil.h"

#include "GPU.h"

struct Gwn_ShaderInterface;

namespace bWidgetsDemo {

class GPUShader {
 public:
  typedef enum {
    ID_UNIFORM_COLOR,
    ID_SMOOTH_COLOR,
    ID_BITMAP_TEXTURE_UNIFORM_COLOR,
    ID_SUBPIXEL_BITMAP_TEXTURE_UNIFORM_COLOR,
    ID_TEXTURE_RECT,

    SHADER_ID_TOT
  } ID;

  ~GPUShader();

  static void clearCache();

  static void immBind(ID id);
  static void immUnbind();

 private:
  GPUShader(const std::string& vertexcode, const std::string& fragcode);

  static std::unique_ptr<GPUShader> createFromID(GPUShader::ID id);
  static GPUShader& ensureCached(ID id);

  GLuint m_program;

  GLuint m_vertex;
  GLuint m_fragment;

  Gwn_ShaderInterface* m_interface;
};

}  // namespace bWidgetsDemo