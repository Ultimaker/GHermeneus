cmake_minimum_required(VERSION 3.17)
project(GHermeneus)

set(SRC_FILES
        src/Machine.cpp
        src/GCode.cpp)

set(HDR_FILES
        include/GHermeneus/Machine.h
        include/GHermeneus/GCode.h)

add_library(GHermeneus SHARED ${SRC_FILES} ${HDR_FILES})
conan_target_link_libraries(GHermeneus)
target_compile_features(GHermeneus PRIVATE cxx_std_17)
