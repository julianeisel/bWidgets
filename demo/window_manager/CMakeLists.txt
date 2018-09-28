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
	${FREETYPE_INCLUDE_DIRS} # XXX Only needed because of ugly freetype includes in Font.h
	${GPU_INC}
	..
	../gpu
)

set(SRC
	Event.cc
	EventManager.cc
	Window.cc
	WindowManager.cc

	Event.h
	EventManager.h
	Window.h
	WindowManager.h
)

set(LIB
	bwd_gpu
	bwd_extern_glew
)

add_library(bwd_window_manager ${SRC})
include_directories(${INC})
target_link_libraries(bwd_window_manager ${LIB})
