set(SRC
	bwColor.cc
	bwGradient.cc
	bwPoint.cc
	bwPolygon.cc
	bwRange.cc
	bwRectangle.cc

	bwColor.h
	bwDistance.h
	bwGradient.h
	bwPoint.h
	bwPolygon.h
	bwRange.h
	bwRectangle.h
)

add_library(bw_generics ${SRC})
