if(NOT WITH_GTESTS)
	message(WARNING "Compiling tests even though WITH_GTESTS disabled")
endif()

if(CMAKE_COMPILER_IS_GNUCC)
	remove_cc_flag(
		"-Wundef"
	)
endif()

set(INC
	../../bwidgets/generics
	../gtest/include
)

set(SRC
	bwPolygon_test.cc
)

set(LIB
	bw_generics
	testing_gtest
	testing_main
)

set(SYS_LIB
	-lpthread
)

add_executable(testing_bwidgets ${SRC})
target_link_libraries(testing_bwidgets ${LIB} ${SYS_LIB})
include_directories(${INC})

add_test(NAME bwidgets_bwpolygon_test COMMAND testing_bwidgets)
