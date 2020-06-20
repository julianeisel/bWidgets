# No source files here, causing error on clang.
if(False)
  set(SRC
    bwUtil.h
  )

  add_library(bw_utils ${SRC})
  set_target_properties(bw_utils PROPERTIES LINKER_LANGUAGE CXX)
endif()
