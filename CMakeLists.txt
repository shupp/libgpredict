cmake_minimum_required(VERSION 2.8)

project (gpredict)
set(CMAKE_BUILD_TYPE Release)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(APPLE)
    set(CMAKE_MACOSX_RPATH ON)
endif()

# Glib
# cmake/FindGlib.cmake is taken from https://github.com/WebKit/webkit/blob/master/Source/cmake/FindGLIB.cmake
find_package(Glib REQUIRED)
include_directories(${GLIB_INCLUDE_DIR})
include_directories(${GLIBCONFIG_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(gpredict SHARED
    sgpsdp/solar.c
    sgpsdp/sgp_obs.c
    sgpsdp/sgp_in.c
    sgpsdp/sgp_time.c
    sgpsdp/sgp_math.c
    sgpsdp/sgp4sdp4.c
    predict-tools.c
    time-tools.c
    orbit-tools.c
    gtk-sat-data.c
    qth-data.c
    sat-vis.c
)

# link glib
target_link_libraries(gpredict ${GLIB_LIBRARIES})

install (TARGETS gpredict
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin
)

INSTALL(FILES
        gpredict.h
        DESTINATION "include/gpredict"
 )