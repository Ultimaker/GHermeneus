cmake_minimum_required(VERSION 3.17)
project(GHermeneus)
set(GHermeneus_VERSION_MAJOR 0)
set(GHermeneus_VERSION_MINOR 1)
set(GHermeneus_VERSION_PATCH 0)

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE OFF) # easier to debug cmake
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)  # cmake find files

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17") # compile with C++17 standard and threading support

add_subdirectory(GHermeneus)

option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")
if (ENABLE_UNIT_TESTS)
    add_subdirectory(tests)
endif ()