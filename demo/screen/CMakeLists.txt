set(INC
	${GPU_INC}
	${FREETYPE_INCLUDE_DIRS}
	../gpu
	../../bwidgets/util
)

set(SRC
	default_stage.cc
	Font.cc
	Stage.cc

	default_stage.h
	Font.h
	Stage.h
)

set(LIB
	${FREETYPE_LIBRARIES}
	bWidgets
	bw_gpu
)

add_library(bw_screen ${SRC})
include_directories(bw_screen ${INC})
target_link_libraries(bw_screen ${LIB})
