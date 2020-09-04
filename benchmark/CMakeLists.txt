message(STATUS)
message(STATUS "==== Benchmark programs ====")
message(STATUS)

set(TESTS
        bm_machine)

foreach(PROGRAM ${TESTS})
    message(STATUS "... add ${PROGRAM}")
    add_executable(${PROGRAM} "${PROGRAM}.cpp")
    source_group("" FILES "${PROGRAM}.cpp")
    conan_target_link_libraries(${PROGRAM})
endforeach(PROGRAM)
