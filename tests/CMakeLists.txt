project(GHermeneus_tests)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.8.1
)

FetchContent_GetProperties(googletest)

if (NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    set(gtest_force_shared_crt ON CACHE BOOL " " FORCE)
    set(gtest_disable_pthreads ON CACHE BOOL " " FORCE)
    add_subdirectory(
            ${googletest_SOURCE_DIR}
            ${googletest_BINARY_DIR}
    )
endif ()

include_directories(../GHermeneus/include)

set(SRC_FILES
        main.cpp)

add_executable(GHermeneus_tests "")
target_sources(GHermeneus_tests
        PRIVATE
        ${SRC_FILES})
target_link_libraries(GHermeneus_tests
        PRIVATE
        GHermeneus
        gtest_main)
enable_testing()
add_test(NAME google_test
        COMMAND $<TARGET_FILE:GHermeneus_tests>)