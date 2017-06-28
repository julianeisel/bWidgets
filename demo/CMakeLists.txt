set(INC
	../bwidgets
	screen
	window_manager
)

set(SRC
	Application.cc
	bwidgets_demo.cc

	Application.h
)

set(LIB
	bw_gpu
	bw_window_manager
	bw_screen
)

set(GPU_INC
	../../extern/glfw/include
	../../extern/glew/include
	../../extern/glm
)

add_executable(bWidgetsDemo ${SRC})
include_directories(bWidgetsDemo ${INC})
target_link_libraries(bWidgetsDemo LINK_PUBLIC ${LIB})

add_subdirectory(window_manager)
add_subdirectory(screen)
add_subdirectory(gpu)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lGL")
