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
	include
)

set(SRC
	src/glew.c
#	src/glewinfo.c
#	src/visualinfo.c

	include/GL/eglew.h
	include/GL/glew.h
	include/GL/glxew.h
	include/GL/wglew.h
)

set(LIB
  ${OPENGL_LIBRARIES}
)

add_library(bwd_extern_glew ${SRC})
include_directories(bwd_extern_glew ${INC})
target_link_libraries(bwd_extern_glew ${LIB})
