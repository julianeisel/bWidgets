set(INC
	${GPU_INC}
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
	extern_glew
)

add_library(bwd_window_manager ${SRC})
include_directories(${INC})
target_link_libraries(bwd_window_manager ${LIB})
