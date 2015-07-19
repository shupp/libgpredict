cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

project(iss-test)

add_executable(iss-test
	iss-test.c
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake")

# Glib
# cmake/FindGlib.cmake is taken from https://github.com/WebKit/webkit/blob/master/Source/cmake/FindGLIB.cmake
find_package(Glib REQUIRED)
include_directories(${GLIB_INCLUDE_DIR})
include_directories(${GLIBCONFIG_INCLUDE_DIR})

target_link_libraries(iss-test gpredict)

# sys math lib
target_link_libraries(iss-test m)

install(TARGETS iss-test DESTINATION bin)