cmake_minimum_required(VERSION 3.17)
project(custom_hash_map)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(lib/googletest-release-1.10.0)
include_directories(lib/googletest-release-1.10.0/googletest/include)
include_directories(lib/googletest-release-1.10.0/googlemock/include)

add_executable(custom_hash_map main.cpp Tests/tests.h HashMap/hash_map.h)
target_link_libraries(custom_hash_map gtest gtest_main)