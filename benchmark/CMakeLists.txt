message(STATUS)
message(STATUS "==== Benchmark programs ====")
message(STATUS)

set(TESTS
        bm_machine)

include_directories(${PROJECT_SOURCE_DIR}/GHermeneus/include)

foreach(PROGRAM ${TESTS})
    message(STATUS "... add ${PROGRAM}")
    add_executable(${PROGRAM} "${PROGRAM}.cpp")
    add_custom_command(
            TARGET ${PROGRAM} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
            DEPENDS ${destination}
            COMMENT "symbolic link resources folder from ${source} => ${destination}"
    )
    source_group("" FILES "${PROGRAM}.cpp")
    conan_target_link_libraries(${PROGRAM})
    target_link_libraries(${PROGRAM} GHermeneus)
    target_compile_features(${PROGRAM} PRIVATE cxx_std_20)
endforeach(PROGRAM)
