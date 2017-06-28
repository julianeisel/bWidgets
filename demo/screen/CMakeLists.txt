set(INC
	${GPU_INC}
	../gpu
)

set(SRC
	default_stage.cc
	Stage.cc

	default_stage.h
	Stage.h
)

set(LIB
	bWidgets
)

add_library(bw_screen ${SRC})
include_directories(bw_screen ${INC})
target_link_libraries(bw_screen ${LIB})
