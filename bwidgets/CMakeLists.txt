set(INC
	../extern/glfw/include
)

set(SRC
	ActionButtonWidget.cc
	ShaderProgram.cc
	util/Polygon.cc
	util/Point.cc
	Widget.cc

	ActionButtonWidget.h
	ShaderProgram.h
	util/Polygon.h
	util/Point.h
	Widget.h
)

add_library(bWidgets ${SRC})
include_directories(bWidgets ${INC})


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
