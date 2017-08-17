set(INC
	.
	generics
	styles
)

set(SRC
	Label.cc
	Painter.cc
	PushButton.cc
	RadioButton.cc
	Style.cc
	StyleManager.cc
	Widget.cc
	styles/StyleBlenderClassic.cc
	styles/StyleBlenderFlat.cc

	Label.h
	Painter.h
	PushButton.h
	RadioButton.h
	Style.h
	StyleManager.h
	Widget.h
	styles/StyleBlenderClassic.h
	styles/StyleBlenderFlat.h
)

set(LIB
	bw_generics
)

add_library(bWidgets ${SRC})
include_directories(${INC})
target_link_libraries(bWidgets ${LIB})

add_subdirectory(generics)
