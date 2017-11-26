set(INC
	${GPU_INC}
	${FREETYPE_INCLUDE_DIRS}
	..
	../gpu
	../../bwidgets/styles
)

set(SRC
	DefaultStage.cc
	Font.cc
	GawainPaintEngine.cc
	Layout.cc
	Stage.cc

	DefaultStage.h
	Font.h
	GawainPaintEngine.h
	Layout.h
	Stage.h
)

set(LIB
	${FREETYPE_LIBRARIES}
	bWidgets

	bwd_gpu
)

add_library(bwd_screen ${SRC})
include_directories(${INC})
target_link_libraries(bwd_screen ${LIB})
