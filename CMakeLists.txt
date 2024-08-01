cmake_minimum_required(VERSION 3.24)

project(
  crypt
  VERSION 1.0
  LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 11)
string(REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main gmock_main)

include_directories(headers bits.h)
add_library(crypt_library SHARED bits.cc)

enable_testing()

add_executable(
    bits_test
    bits_test.cc
)
target_include_directories(bits_test PRIVATE headers)
target_link_libraries(
    bits_test
    GTest::GTest
    crypt_library
)

add_test(
    NAME bits_test
    COMMAND bits_test
)
