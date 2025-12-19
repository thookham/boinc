# zip/CMakeLists.txt

add_library(libboinc_zip STATIC boinc_zip.cpp)

target_link_libraries(libboinc_zip PUBLIC libboinc libzip::zip)

target_include_directories(libboinc_zip PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib
)
