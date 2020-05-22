project(GHermeneus VERSION 0.1.0 LANGUAGES CXX)

set(SRC_FILES
        src/Machine.cpp
        src/State.cpp)

set(HDR_FILES
        include/GHermeneus/Machine.h
        include/GHermeneus/State.h)

add_library(GHermeneus SHARED ${SRC_FILES} ${HDR_FILES})

target_include_directories(GHermeneus PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)