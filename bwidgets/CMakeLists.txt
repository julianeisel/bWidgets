set(INC
	util
)

set(SRC
	Painter.cc
	PushButton.cc
	Widget.cc
	util/Color.cc
	util/Polygon.cc
	util/Point.cc
	util/Rectangle.cc

	Painter.h
	PushButton.h
	Widget.h
	util/Color.h
	util/Polygon.h
	util/Point.h
	util/Rectangle.h
)

add_library(bWidgets ${SRC})
include_directories(${INC})
