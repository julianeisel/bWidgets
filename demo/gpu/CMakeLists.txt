set(INC
#	../../extern/gawain/gawain
	${GPU_INC}
)

set(SRC
	gpu.cc
	gpu_matrix.cc
	ShaderProgram.cc

	GPU.h
	gpu_intern.h
	ShaderProgram.h
)

set(LIB
	extern_gawain
	extern_glew
	glfw
)

add_library(bw_gpu ${SRC})
include_directories(bw_gpu ${INC})
target_link_libraries(bw_gpu ${LIB})

# ----- shaders -----

set(SHADERS
	uniform_color_vert.glsl
	uniform_color_frag.glsl
)

set(SHADER_PATH ${CMAKE_BINARY_DIR}/bin/shaders)
add_definitions(-DSHADER_PATH_STR="${SHADER_PATH}")

foreach(SHADER ${SHADERS})
	# copy shaders files to build (${CMAKE_BINEARY_DIR}/bin/shaders)
	configure_file(shaders/${SHADER} SHADER_PATH/${SHADER} COPYONLY)
endforeach()
#set_target_properties(bw_gpu PROPERTIES LINKER_LANGUAGE CXX)
