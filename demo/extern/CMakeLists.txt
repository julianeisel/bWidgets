set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(GLM_STATIC_LIBRARY_ENABLE ON CACHE BOOL "" FORCE)

add_definitions(-DGLEW_STATIC)

add_subdirectory(gawain)
add_subdirectory(glew)
add_subdirectory(glfw)
add_subdirectory(glm)
