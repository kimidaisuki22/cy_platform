
enable_testing()
find_package(GTest CONFIG REQUIRED)

add_executable(
  hello_test
  test.cpp
)
target_link_libraries(
  hello_test
  PRIVATE
  GTest::gtest GTest::gtest_main cy_platform
)
add_test(test_A hello_test)
