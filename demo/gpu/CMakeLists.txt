set(INC
	${GPU_INC}
)

set(SRC
	gpu.cc
	gpu_matrix.cc
	ShaderProgram.cc

	GPU.h
	gpu_intern.h
	ShaderProgram.h

	shaders/bitmap_texture_uniform_color_frag.glsl
	shaders/texture_vert.glsl
	shaders/smooth_color_frag.glsl
	shaders/smooth_color_vert.glsl
	shaders/uniform_color_frag.glsl
	shaders/uniform_color_vert.glsl
)

set(LIB
	bwd_extern_gawain
	bwd_extern_glew
	glfw
)

add_library(bwd_gpu ${SRC})
include_directories(${INC})
target_link_libraries(bwd_gpu ${LIB})

# ----- shaders -----

if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
	set(SHADERS
		bitmap_texture_uniform_color_frag.glsl
		texture_vert.glsl
		smooth_color_frag.glsl
		smooth_color_vert.glsl
		uniform_color_vert.glsl
		uniform_color_frag.glsl
	)

	set(SHADERS_PATH ${CMAKE_CURRENT_BINARY_DIR}/shaders)
	foreach(SHADER ${SHADERS})
		# copy shader files to build
		add_custom_command(
			TARGET bwd_gpu PRE_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
					${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SHADER}
					${SHADERS_PATH}/${SHADER}
		)
	endforeach()
else()
	set(SHADERS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
endif()
add_definitions(-DSHADERS_PATH_STR="${SHADERS_PATH}")

#set_target_properties(bwd_gpu PROPERTIES LINKER_LANGUAGE CXX)
