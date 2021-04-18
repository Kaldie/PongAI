enable_testing()

add_executable(test hello_test.cpp)
target_link_libraries(test gtest_main)

include(GoogleTest)
gtest_discover_tests(test)