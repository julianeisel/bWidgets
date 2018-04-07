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
	../bwidgets
	../bwidgets/generics
	../bwidgets/widgets
	screen
	window_manager
)

set(SRC
	Application.cc
	bwidgets_demo.cc
	File.cc

	Application.h
	File.h
)

set(LIB
	bwd_gpu
	bwd_window_manager
	bwd_screen
)

set(GPU_INC
	../extern/glfw/include
	../extern/glew/include
	../extern/glm
)

find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)
list(APPEND LIB ${OPENGL_LIBRARIES})

add_executable(bWidgetsDemo ${SRC})
include_directories(${INC})
target_link_libraries(bWidgetsDemo LINK_PUBLIC ${LIB})

# --- Resource files ---

if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
	set(RESOURCES
		bfont.ttf
		LICENSE-bfont.ttf.txt
		classic_style.css
		flat_light.css
		flat_dark.css
	)

	set(RESOURCES_PATH ${CMAKE_CURRENT_BINARY_DIR}/resources)
	foreach(RES ${RESOURCES})
		# copy resource files to build
		add_custom_command(
			TARGET bWidgetsDemo PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
					${CMAKE_CURRENT_SOURCE_DIR}/resources/${RES}
					${RESOURCES_PATH}/${RES}
		)
	endforeach()
else()
	set(RESOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources)
endif()
add_definitions(-DRESOURCES_PATH_STR="${RESOURCES_PATH}")

# --- subdirectories ---

add_subdirectory(extern)
add_subdirectory(gpu)
add_subdirectory(screen)
add_subdirectory(window_manager)
