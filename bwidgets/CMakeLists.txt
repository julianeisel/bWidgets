set(INC
)

set(SRC
	ActionButtonWidget.cc
	util/Polygon.cc
	util/Point.cc
	Widget.cc

	ActionButtonWidget.h
	util/Polygon.h
	util/Point.h
	Widget.h
)

add_library(bWidgets ${SRC})
include_directories(bWidgets ${INC})
