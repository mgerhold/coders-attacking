include(CTest)
include(GoogleTest)

add_executable(coders_attacking_tests tests.cpp)

target_link_libraries(coders_attacking_tests PRIVATE GTest::gtest GTest::gtest_main)

gtest_discover_tests(coders_attacking_tests)
