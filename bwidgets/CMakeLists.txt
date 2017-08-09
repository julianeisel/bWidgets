set(INC
	.
	util
)

set(SRC
	Painter.cc
	PushButton.cc
	Style.cc
	Widget.cc
	styles/StyleBlenderClassic.cc
	util/Color.cc
	util/Gradient.cc
	util/Polygon.cc
	util/Point.cc
	util/Rectangle.cc

	Painter.h
	PushButton.h
	Style.h
	Widget.h
	styles/StyleBlenderClassic.h
	util/Color.h
	util/Gradient.h
	util/Polygon.h
	util/Point.h
	util/Rectangle.h
)

add_library(bWidgets ${SRC})
include_directories(${INC})
