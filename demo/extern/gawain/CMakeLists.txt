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
	../glew/include
	gawain
)

set(SRC
	src/attrib_binding.c
	src/batch.c
	src/element.c
	src/buffer_id.cpp
	src/immediate.c
	src/imm_util.c
	src/primitive.c
	src/shader_interface.c
	src/vertex_buffer.c
	src/vertex_format.c

	gawain/attrib_binding.h
	gawain/batch.h
	gawain/buffer_id.h
	gawain/common.h
	gawain/element.h
	gawain/immediate.h
	gawain/imm_util.h
	gawain/primitive.h
	gawain/shader_interface.h
	gawain/vertex_buffer.h
	gawain/vertex_format.h
)

set(LIB
	bwd_extern_glew
	bwd_gpu
)

add_definitions(-DWITH_GL_PROFILE_CORE)

add_library(bwd_extern_gawain ${SRC})
include_directories(${INC})
target_link_libraries(bwd_extern_gawain ${LIB})
