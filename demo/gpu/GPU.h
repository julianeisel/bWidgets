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
 * Original work Copyright (c) 2018 Jason Wilkins, Julian Eisel, Mike Erwin
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"


typedef enum GPUShaderID {
	GPU_SHADER_2D_UNIFORM_COLOR,
} GPUShaderID;

void GPU_init(void);
void GPU_exit(void);

void gpuTranslate2f(const float x, const float y);
void gpuTranslate2f(const float vec[2]);

void gpuOrtho(
        float left, float right,
        float bottom, float top);
