if(NOT WITH_GTESTS)
	message(WARNING "Compiling tests even though WITH_GTESTS disabled")
endif()

if(CMAKE_COMPILER_IS_GNUCC)
	remove_cc_flag(
		"-Wundef"
	)
endif()

set(INC
	..
	../../bwidgets
	../../bwidgets/generics
	../../bwidgets/utils
	../gtest/include
)

set(SRC
	bwPolygon_test.cc
	bwStyleProperties_test.cc
	utils/bwOptional/constructors.cc
	utils/bwOptional/comparison_operators.cc
	utils/bwOptional/destructor.cc
	utils/bwOptional/functions.cc
	utils/bwOptional/hash.cc
	utils/bwOptional/make_optional.cc
	utils/bwOptional/operators.cc
	utils/bwOptional/swap.cc
)

set(LIB
	bWidgets
	testing
	testing_gtest
)

set(SYS_LIB
	-lpthread
)

add_executable(testing_bwidgets ${SRC})
target_link_libraries(testing_bwidgets ${LIB} ${SYS_LIB})
include_directories(${INC})

add_test(NAME bwidgets_bwpolygon_test COMMAND testing_bwidgets)
