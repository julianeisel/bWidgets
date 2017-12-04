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

	Application.h
)

set(LIB
	bwd_gpu
	bwd_window_manager
	bwd_screen
)

set(GPU_INC
	../../extern/glfw/include
	../../extern/glew/include
	../../extern/glm
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

add_subdirectory(window_manager)
add_subdirectory(screen)
add_subdirectory(gpu)
