cmake_minimum_required(VERSION 2.8.12)

# Avoid having emtpy build type
set(CMAKE_BUILD_TYPE_INIT "Release")

#-----------------------------------------------------------------------------
# Initialize Project

project(bWidgets)

#-----------------------------------------------------------------------------
# Redirect output files

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

#-----------------------------------------------------------------------------
# Sub-directories

add_subdirectory(bwidgets)
add_subdirectory(demo)
add_subdirectory(extern)
