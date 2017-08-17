set(SRC
	bwColor.cc
	bwGradient.cc
	bwPolygon.cc
	bwPoint.cc
	bwRectangle.cc

	bwColor.h
	bwGradient.h
	bwPolygon.h
	bwPoint.h
	bwRectangle.h
)

add_library(bw_generics ${SRC})
