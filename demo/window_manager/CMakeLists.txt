set(INC
	${GPU_INC}
	../gpu
)

set(SRC
	EventManager.cc
	Window.cc
	WindowManager.cc

	EventManager.h
	Window.h
	WindowManager.h
)

set(LIB
	bw_gpu
	extern_glew
)

add_library(bw_window_manager ${SRC})
include_directories(bw_window_manager ${INC})
target_link_libraries(bw_window_manager ${LIB})
