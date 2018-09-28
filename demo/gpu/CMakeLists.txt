# ***** BEGIN GPL LICENSE BLOCK *****
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, see <http://www.gnu.org/licenses/>.
#
# Original work Copyright (c) 2018 Julian Eisel
#
# ***** END GPL LICENSE BLOCK *****

set(INC
	..
	${GPU_INC}
)

set(SRC
	gpu.cc
	gpu_matrix.cc
	ShaderProgram.cc

	GPU.h
	gpu_intern.h
	ShaderProgram.h

	shaders/pixel_alpha_mask_texture_uniform_color_frag.glsl
	shaders/subpixel_alpha_mask_texture_uniform_color_frag.glsl
	shaders/texture_frag.glsl
	shaders/texture_vert.glsl
	shaders/smooth_color_frag.glsl
	shaders/smooth_color_vert.glsl
	shaders/uniform_color_frag.glsl
	shaders/uniform_color_vert.glsl
)

set(LIB
	bwd_extern_gawain
	bwd_extern_glew
	glfw
)

add_library(bwd_gpu ${SRC})
include_directories(${INC})
target_link_libraries(bwd_gpu ${LIB})

# ----- shaders -----

if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
	set(SHADERS
		pixel_alpha_mask_texture_uniform_color_frag.glsl
		subpixel_alpha_mask_texture_uniform_color_frag.glsl
		texture_frag.glsl
		texture_vert.glsl
		smooth_color_frag.glsl
		smooth_color_vert.glsl
		uniform_color_vert.glsl
		uniform_color_frag.glsl
	)

	set(SHADERS_PATH ${CMAKE_CURRENT_BINARY_DIR}/shaders)
	foreach(SHADER ${SHADERS})
		# copy shader files to build
		add_custom_command(
			TARGET bwd_gpu PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
					${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SHADER}
					${SHADERS_PATH}/${SHADER}
		)
	endforeach()
else()
	set(SHADERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
endif()
add_definitions(-DSHADERS_PATH_STR="${SHADERS_PATH}")

#set_target_properties(bwd_gpu PROPERTIES LINKER_LANGUAGE CXX)
