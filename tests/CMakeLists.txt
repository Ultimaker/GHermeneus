include_directories(../GHermeneus/include)

set(SRC_FILES
        state_test.cpp
        main.cpp)

add_executable(GHermeneus_tests "")
target_sources(GHermeneus_tests PRIVATE ${SRC_FILES})
conan_target_link_libraries(GHermeneus_tests GHermeneusLib CONAN_PKG::gtest)

enable_testing()
add_test(NAME google_test
        COMMAND $<TARGET_FILE:GHermeneus_tests>)