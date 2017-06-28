set(INC
#	../../extern/gawain/gawain
	${GPU_INC}
)

set(SRC
	gpu.cc
	gpu_matrix.cc

	GPU.h
	gpu_intern.h
)

set(LIB
	extern_gawain
	extern_glew
	glfw
)

add_library(bw_gpu ${SRC})
include_directories(bw_gpu ${INC})
target_link_libraries(bw_gpu ${LIB})

#set_target_properties(bw_gpu PROPERTIES LINKER_LANGUAGE CXX)
