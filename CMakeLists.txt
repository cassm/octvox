cmake_minimum_required(VERSION 2.8.4)
project(octvox)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/OctLeaf.cpp
    README.md)

set(TEST_SOURCE_FILES
    test/main.cpp
    test/OctLeafTest.cpp
    test/OctreeTest.cpp)

set(GTEST_ROOT test/googletest)

add_executable(octvox_test ${TEST_SOURCE_FILES} ${SOURCE_FILES})
add_executable(octvox ${SOURCE_FILES} src/main.cpp)

include_directories(${GTEST_ROOT}/include)
include_directories(src/include)

add_subdirectory(${GTEST_ROOT})

target_link_libraries(octvox_test gtest)