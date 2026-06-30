cmake_minimum_required(VERSION 3.10.0)
project(proj VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  proj
  view_test.cpp
  iter_test.cpp
)
target_link_libraries(
  proj
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(proj)
