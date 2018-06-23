if(CMAKE_COMPILER_IS_GNUCC)
	remove_cc_flag(
		"-Wundef"
	)
endif()

set(INC
	../gtest/include
)

set(SRC
	testing_main.cc

	TestUtilClasses.h
	TestUtilMacros.h
)

add_library(testing_main ${SRC})
include_directories(${INC})
