set(SRC
	bwColor.cc
	bwGradient.cc
	bwPoint.cc
	bwPolygon.cc

	bwColor.h
	bwDistance.h
	bwGradient.h
	bwFunctorInterface.h
	bwPoint.h
	bwPolygon.h
	bwRange.h
	bwRectangle.h
)

add_library(bw_generics)
target_sources(bw_generics PRIVATE ${SRC})
