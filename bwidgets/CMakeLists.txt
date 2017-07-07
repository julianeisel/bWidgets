set(INC
)

set(SRC
	PushButton.cc
	util/Polygon.cc
	util/Point.cc
	Widget.cc

	PushButton.h
	util/Polygon.h
	util/Point.h
	Widget.h
)

add_library(bWidgets ${SRC})
include_directories(bWidgets ${INC})
