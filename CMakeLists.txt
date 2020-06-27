cmake_minimum_required(VERSION 3.1)

# Avoid having emtpy build type
set(CMAKE_BUILD_TYPE_INIT "Release")

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

macro(remove_cc_flag _flag)
	foreach(flag ${ARGV})
		string(REGEX REPLACE ${flag} "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
		string(REGEX REPLACE ${flag} "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
		string(REGEX REPLACE ${flag} "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
		string(REGEX REPLACE ${flag} "" CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}")
		string(REGEX REPLACE ${flag} "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")

		string(REGEX REPLACE ${flag} "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
		string(REGEX REPLACE ${flag} "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
		string(REGEX REPLACE ${flag} "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
		string(REGEX REPLACE ${flag} "" CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")
		string(REGEX REPLACE ${flag} "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
	endforeach()
	unset(flag)
endmacro()


#-----------------------------------------------------------------------------
# Set default warnings

function(ADD_CHECK_CXX_COMPILER_FLAG
	_CXXFLAGS
	_CACHE_VAR
	_FLAG
	)

	include(CheckCXXCompilerFlag)

	CHECK_CXX_COMPILER_FLAG("${_FLAG}" "${_CACHE_VAR}")
	if(${_CACHE_VAR})
		# message(STATUS "Using CXXFLAG: ${_FLAG}")
		set(${_CXXFLAGS} "${${_CXXFLAGS}} ${_FLAG}" PARENT_SCOPE)
	else()
		message(STATUS "Unsupported CXXFLAG: ${_FLAG}")
	endif()
endfunction()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_ALL -Wall)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_NO_INVALID_OFFSETOF -Wno-invalid-offsetof)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_NO_SIGN_COMPARE -Wno-sign-compare)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_LOGICAL_OP -Wlogical-op)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_INIT_SELF -Winit-self)  # needs -Wuninitialized
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_MISSING_INCLUDE_DIRS -Wmissing-include-dirs)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_NO_DIV_BY_ZERO -Wno-div-by-zero)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_TYPE_LIMITS -Wtype-limits)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_ERROR_RETURN_TYPE  -Werror=return-type)
#	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_ERROR_IMPLICIT_FUNCTION_DECLARATION -Werror=implicit-function-declaration)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_NO_CHAR_SUBSCRIPTS -Wno-char-subscripts)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_NO_UNKNOWN_PRAGMAS -Wno-unknown-pragmas)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_POINTER_ARITH -Wpointer-arith)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_UNUSED_PARAMETER -Wunused-parameter)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_WRITE_STRINGS -Wwrite-strings)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_UNDEF -Wundef)
	ADD_CHECK_CXX_COMPILER_FLAG(CXX_WARNINGS CXX_WARN_FORMAT_SIGN -Wformat-signedness)
elseif (MSVC)
    set(_WARNINGS
        # warning level:
        "/W3"
        # disable:
        "/wd4244"  # conversion from 'type1' to 'type2', possible loss of data
    )

    string(REPLACE ";" " " _WARNINGS "${_WARNINGS}")
    set(CXX_WARNINGS "${_WARNINGS}")
    unset(_WARNINGS)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_WARNINGS}")

#-----------------------------------------------------------------------------
# Initialize Project

project(bWidgets)

enable_testing()

#-----------------------------------------------------------------------------
# Redirect output files

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

#-----------------------------------------------------------------------------
# Compile options

option(WITH_GTESTS "Compile bWidgets with GTest unit tests" OFF)
option(WITH_BWIDGETS_DEMO "Compile the demo application for bWidgets alongside bWidgets itself" ON)

#-----------------------------------------------------------------------------
# Sub-directories

add_subdirectory(bwidgets)

if(WITH_BWIDGETS_DEMO)
	add_subdirectory(demo)
endif()
if(WITH_GTESTS)
	add_subdirectory(tests)
endif()
