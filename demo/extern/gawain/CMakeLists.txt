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

# WITH_OPENGL limits the visibility of the opengl headers to just gawain and bg_gpu,
# to more easily highlight codepadths in other libraries that need to be refactored,
# bf_intern_gawain is allowed to have opengl regardless of this option.


set(INC
	../glew/include
	${PTHREADS_INCLUDE_DIRS}
	gawain
)

set(SRC
	src/gwn_attr_binding.c
	src/gwn_batch.c
	src/gwn_element.c
	src/gwn_buffer_id.cpp
	src/gwn_immediate.c
	src/gwn_imm_util.c
	src/gwn_primitive.c
	src/gwn_shader_interface.c
	src/gwn_vertex_array_id.cpp
	src/gwn_vertex_buffer.c
	src/gwn_vertex_format.c

	gawain/gwn_attr_binding.h
	gawain/gwn_attr_binding_private.h
	gawain/gwn_batch.h
	gawain/gwn_batch_private.h
	gawain/gwn_buffer_id.h
	gawain/gwn_common.h
	gawain/gwn_element.h
	gawain/gwn_imm_util.h
	gawain/gwn_immediate.h
	gawain/gwn_primitive.h
	gawain/gwn_primitive_private.h
	gawain/gwn_shader_interface.h
	gawain/gwn_vertex_array_id.h
	gawain/gwn_vertex_buffer.h
	gawain/gwn_vertex_format.h
	gawain/gwn_vertex_format_private.h
)

set(LIB
	bwd_extern_glew
	${PTHREADS_LIBRARIES}
)

add_definitions(-DWITH_GL_PROFILE_CORE -DWITH_OPENGL)

add_library(bwd_extern_gawain ${SRC})
include_directories(${INC})
target_link_libraries(bwd_extern_gawain ${LIB})
