include_directories(../GHermeneus/include)

set(SRC_FILES
        machine_test.cpp
        state_test.cpp
        main.cpp)

add_executable(GHermeneus_tests "")
target_sources(GHermeneus_tests PRIVATE ${SRC_FILES})
target_link_libraries(GHermeneus_tests GHermeneus)
conan_target_link_libraries(GHermeneus_tests CONAN_PKG::gtest)
target_compile_features(GHermeneus_tests PRIVATE cxx_std_17)

enable_testing()
add_test(NAME google_test
        COMMAND $<TARGET_FILE:GHermeneus_tests>)