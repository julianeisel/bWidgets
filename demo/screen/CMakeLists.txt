set(INC
	${GPU_INC}
	${FREETYPE_INCLUDE_DIRS}
	../gpu
	../../bwidgets/util
)

set(SRC
	DefaultStage.cc
	Font.cc
	Stage.cc

	DefaultStage.h
	Font.h
	Stage.h
)

set(LIB
	${FREETYPE_LIBRARIES}
	bWidgets
	bw_gpu
)

add_library(bw_screen ${SRC})
include_directories(${INC})
target_link_libraries(bw_screen ${LIB})
