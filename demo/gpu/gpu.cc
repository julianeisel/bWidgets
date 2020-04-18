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

#include <cassert>
#include <iostream>

extern "C" {
#include "gawain/gwn_immediate.h"
}

#include "GPU.h"

static bool initialized = false;

/**
 * \return true if successful.
 */
void GPU_init()
{
  if (initialized) {
    assert(0);
    return;
  }
  initialized = true;

  GLenum glew_ret = glewInit();
  if (glew_ret != GLEW_OK) {
    std::cout << "Error: " << glewGetErrorString(glew_ret) << std::endl;
    assert(0);
  }
  immInit();
}

void GPU_exit()
{
  immDestroy();
}
