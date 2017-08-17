set(INC
	.
	generics
	styles
)

set(SRC
	bwAbstractButton.cc
	bwLabel.cc
	bwPainter.cc
	bwPushButton.cc
	bwRadioButton.cc
	bwStyle.cc
	bwStyleManager.cc
	bwWidget.cc
	styles/bwStyleBlenderClassic.cc
	styles/bwStyleBlenderFlat.cc

	bwAbstractButton.h
	bwLabel.h
	bwPainter.h
	bwPushButton.h
	bwRadioButton.h
	bwStyle.h
	bwStyleManager.h
	bwWidget.h
	styles/bwStyleBlenderClassic.h
	styles/bwStyleBlenderFlat.h
)

set(LIB
	bw_generics
)

add_library(bWidgets ${SRC})
include_directories(${INC})
target_link_libraries(bWidgets ${LIB})

add_subdirectory(generics)
