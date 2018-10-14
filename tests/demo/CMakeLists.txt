if(NOT WITH_GTESTS)
	message(WARNING "Compiling tests even though WITH_GTESTS disabled")
endif()

if(CMAKE_COMPILER_IS_GNUCC)
	remove_cc_flag(
		"-Wundef"
	)
endif()

set(INC
	../../bwidgets
	../../bwidgets/generics
	../../bwidgets/utils
	../../bwidgets/widgets
	../../demo/screen
	../gtest/include
)

set(SRC
	WidgetIterator_test.cc
)

set(LIB
	bwd_screen
	testing
	testing_gtest
)

set(SYS_LIB
	-lpthread
)

add_executable(testing_bwidgets_demo ${SRC})
target_link_libraries(testing_bwidgets_demo ${LIB} ${SYS_LIB})
include_directories(${INC})
