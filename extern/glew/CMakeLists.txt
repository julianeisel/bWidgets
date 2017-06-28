set(INC
	include
)

set(SRC
	src/glew.c

	include/GL/eglew.h
	include/GL/glew.h
	include/GL/glxew.h
	include/GL/wglew.h
)

add_library(extern_glew ${SRC})
include_directories(extern_glew ${INC})
