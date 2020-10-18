message(STATUS)
message(STATUS "==== Benchmark programs ====")
message(STATUS)

set(TESTS
        bm_machine)

foreach(PROGRAM ${TESTS})
    message(STATUS "... add ${PROGRAM}")
    add_executable(${PROGRAM} "${PROGRAM}.cpp")
    source_group("" FILES "${PROGRAM}.cpp")
    target_link_libraries(
            ${PROGRAM}
            PRIVATE
                project_options
                project_warnings
                GHermeneus
                CONAN_PKG::benchmark
                CONAN_PKG::spdlog)
endforeach(PROGRAM)
