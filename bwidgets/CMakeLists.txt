set(INC
	.
	generics
)

set(SRC
	Label.cc
	Painter.cc
	PushButton.cc
	Style.cc
	Widget.cc
	styles/StyleBlenderClassic.cc

	Label.h
	Painter.h
	PushButton.h
	Style.h
	Widget.h
	styles/StyleBlenderClassic.h
)

set(LIB
	bw_generics
)

add_library(bWidgets ${SRC})
include_directories(${INC})
target_link_libraries(bWidgets ${LIB})

add_subdirectory(generics)
