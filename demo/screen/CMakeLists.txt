set(INC
	${GPU_INC}
	${FREETYPE_INCLUDE_DIRS}
	..
	../gpu
	../extern/katana-parser/src
	../../bwidgets/styles
)

set(SRC
	DefaultStage.cc
	Font.cc
	GawainPaintEngine.cc
	Layout.cc
	Stage.cc
	StyleSheet.cc
	StyleSheetTree.cc

	DefaultStage.h
	Font.h
	GawainPaintEngine.h
	Layout.h
	Stage.h
	StyleSheet.h
	StyleSheetTree.h
)

set(LIB
	${FREETYPE_LIBRARIES}
	bWidgets
	extern_katana

	bwd_gpu
)

add_library(bwd_screen ${SRC})
include_directories(${INC})
target_link_libraries(bwd_screen ${LIB})
