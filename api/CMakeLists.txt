# api/CMakeLists.txt

# --- libboinc_api ---

set(API_SOURCES
    boinc_api.cpp
    graphics2_util.cpp
    reduce_main.cpp
)

add_library(libboinc_api STATIC ${API_SOURCES})

target_link_libraries(libboinc_api PUBLIC
    libboinc
)

target_include_directories(libboinc_api PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# --- libboinc_graphics2 ---
# (Requires JPEG/GLUT, skipping for initial port if dependencies aren't verified yet)
# But we can define the target and condition it.

if(BOINC_BUILD_GRAPHICS)
    set(GRAPHICS2_SOURCES
        gutil.cpp
        gutil_text.cpp
        reduce_lib.cpp
        graphics2.cpp
    )
    if(WIN32)
       # Add windows specific graphics files if needed
    elseif(UNIX)
       list(APPEND GRAPHICS2_SOURCES graphics2_unix.cpp)
    endif()

    add_library(libboinc_graphics2 STATIC ${GRAPHICS2_SOURCES})
    target_link_libraries(libboinc_graphics2 PUBLIC libboinc_api)
endif()

# --- libboinc_opencl ---

add_library(libboinc_opencl STATIC boinc_opencl.cpp)
target_link_libraries(libboinc_opencl PUBLIC libboinc_api)
