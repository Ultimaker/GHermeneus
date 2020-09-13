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
        include/GHermeneus/Translator.h
        include/GHermeneus/Dialects/Marlin.h
        include/GHermeneus/Utils/Concepts.h)

add_library(
        GHermeneus
        ${SRC_FILES}
        ${HDR_FILES})

target_link_libraries(
        GHermeneus
        PUBLIC
            CONAN_PKG::range-v3
            CONAN_PKG::eigen
            CONAN_PKG::fmt
            CONAN_PKG::spdlog
        PRIVATE
            project_options
            project_warnings
            CONAN_PKG::tbb)

target_include_directories(
        GHermeneus
        PUBLIC
            "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/GHermeneus/include>"
            "$<INSTALL_INTERFACE:include>"
        PRIVATE
            ${CMAKE_SOURCE_DIR}/GHermeneus/src)

if (ENABLE_TESTING OR ENABLE_BENCHMARK)
    add_custom_command(
            TARGET GHermeneus POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink ${resources_src} ${resources_dst}
            DEPENDS ${resources_dst}
            COMMENT "symbolic link resources folder from ${resources_src} => ${resources_dst}"
    )
endif()
