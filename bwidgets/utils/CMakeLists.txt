set(SRC
	bwOptional.h
	bwPtr.h
	bwUtil.h
	internal/bwOptional_internal.h
)

add_library(bw_utils ${SRC})
set_target_properties(bw_utils PROPERTIES LINKER_LANGUAGE CXX)
