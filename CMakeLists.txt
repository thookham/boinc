cmake_minimum_required(VERSION 3.20)

project(BOINC
    VERSION 8.3.0
    LANGUAGES CXX C
)

# Standardize on C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies (via vcpkg)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(libzip CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenCL REQUIRED)

# Options
option(BOINC_BUILD_CLIENT "Build the BOINC Client" ON)
option(BOINC_BUILD_SERVER "Build the BOINC Server" OFF) # Default off for now
option(BOINC_BUILD_MANAGER "Build the BOINC Manager GUI" OFF) # Default off for now
option(BOINC_BUILD_TESTS "Build Unit Tests" OFF)

# Include directories (Global)
include_directories(
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/api
    ${CMAKE_SOURCE_DIR}/db
)

# Subdirectories
add_subdirectory(lib)
add_subdirectory(zip)

if(BOINC_BUILD_CLIENT)
    add_subdirectory(client)
endif()

# add_subdirectory(apps)   # TODO: Port apps

if(BOINC_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/unit-tests)
endif()
