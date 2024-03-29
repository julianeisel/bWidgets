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
	../bwidgets/styling
	../bwidgets/utils
	../bwidgets/widgets
	screen
	window_manager
)

set(SRC
	Application.cc
	Pixmap.cc
	bwidgets_demo.cc
	File.cc

	Application.h
	Pixmap.h
	File.h
	RNAProperty.h
	RNAScreenGraphBuilder.h
	resources/blender_icon_defines.h
)

set(LIB
	bwd_gpu
	bwd_window_manager
	bwd_screen
)

set(GPU_INC
	../extern/glew/include
	../extern/glm
)

if (VCPKG_TOOLCHAIN)
	message(STATUS "Using vcpkg for library dependencies")

	set(FREETYPE_INCLUDE_DIRS "${FREETYPE_INCLUDE_DIR_freetype2} ${FREETYPE_INCLUDE_DIR_ft2build}")
elseif (WIN32)
	if(NOT DEFINED LIBDIR)
		set(LIBDIR ${CMAKE_SOURCE_DIR}/../lib)
	endif()

	if(NOT EXISTS "${LIBDIR}/")
		message(FATAL_ERROR "Windows without vcpkg requires pre-compiled libs at: '${LIBDIR}'")
	endif()
endif()

if(LIBDIR)
	message(STATUS "Pre-compiled libs expected at: '${LIBDIR}'")

	set(ZLIB_INCLUDE_DIR ${LIBDIR}/zlib/include)
	set(ZLIB_LIBRARY ${LIBDIR}/zlib/lib/libz_st.lib)

	set(PNG_PNG_INCLUDE_DIR ${LIBDIR}/png/include)
	set(PNG_LIBRARY ${LIBDIR}/png/lib/libpng.lib)
	set(PNG "${LIBDIR}/png")
	set(PNG_INCLUDE_DIR "${PNG}/include")
	set(PNG_LIBPATH ${PNG}/lib)

	set(FREETYPE ${LIBDIR}/freetype)
	set(FREETYPE_INCLUDE_DIRS ${LIBDIR}/freetype/include/freetype2)
	set(FREETYPE_LIBRARY ${LIBDIR}/freetype/lib/freetype2ST.lib)

	set(PTHREADS_INCLUDE_DIR ${LIBDIR}/pthreads/include)
	set(PTHREADS_LIBRARIES ${LIBDIR}/pthreads/lib/pthreadVC3.lib)
endif()

if (UNIX)
  set(OpenGL_GL_PREFERENCE GLVND)
endif()

find_package(PNG)
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)
if (VCPKG_TOOLCHAIN AND WIN32)
  find_package(PThreads REQUIRED)
endif()

list(APPEND LIB ${OPENGL_LIBRARIES})

add_definitions(-DGLEW_STATIC)

add_executable(bWidgetsDemo)
target_sources(bWidgetsDemo PRIVATE ${SRC})
include_directories(${INC})
target_link_libraries(bWidgetsDemo LINK_PRIVATE ${LIB})

# --- Resource files ---

if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
	set(RESOURCES
		bfont.ttf
		blender_icons16.png
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
add_subdirectory(stylesheet)
add_subdirectory(utils)
add_subdirectory(window_manager)
