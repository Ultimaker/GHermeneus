cmake_minimum_required(VERSION 3.17)
project(GHermeneus)

set(SRC_FILES
        src/Dialects/Marlin.cpp)

set(HDR_FILES
        include/GHermeneus/GHermeneus.h
        include/GHermeneus/Machine.h
        include/GHermeneus/Instruction.h
        include/GHermeneus/StateSpaceVector.h
        include/GHermeneus/GCode.h
        include/GHermeneus/Parameters.h
        include/GHermeneus/Dialects/Marlin.h
        )

add_library(GHermeneus SHARED ${SRC_FILES} ${HDR_FILES})
conan_target_link_libraries(GHermeneus)
target_compile_features(GHermeneus PRIVATE cxx_std_20)
