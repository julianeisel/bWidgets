set(INC
	util
)

set(SRC
	PushButton.cc
	Widget.cc
	util/Polygon.cc
	util/Point.cc
	Painter.cc

	PushButton.h
	Widget.h
	util/Polygon.h
	util/Point.h
	Painter.h
)

add_library(bWidgets ${SRC})
include_directories(bWidgets ${INC})
