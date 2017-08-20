set(INC
	.
	generics
	styles
	widgets
)

set(SRC
	bwPainter.cc
	bwStyle.cc
	bwStyleManager.cc
	styles/bwStyleBlenderClassic.cc
	styles/bwStyleBlenderFlat.cc

	bwPainter.h
	bwStyle.h
	bwStyleManager.h
	styles/bwStyleBlenderClassic.h
	styles/bwStyleBlenderFlat.h
)

set(LIB
	bw_generics
	bw_widgets
)

add_library(bWidgets ${SRC})
include_directories(${INC})
target_link_libraries(bWidgets ${LIB})

add_subdirectory(generics)
add_subdirectory(widgets)
