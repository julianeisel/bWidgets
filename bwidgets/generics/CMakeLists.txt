set(SRC
	bwColor.cc
	bwGradient.cc
	bwPoint.cc
	bwPolygon.cc
	bwRectangle.cc

	bwColor.h
	bwGradient.h
	bwPoint.h
	bwPolygon.h
	bwRectangle.h
)

add_library(bw_generics ${SRC})
