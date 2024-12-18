cmake_minimum_required(VERSION 3.30)
project(parallel1)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(parlaylib
        GIT_REPOSITORY https://github.com/cmuparlay/parlaylib.git
        GIT_TAG master
)
FetchContent_GetProperties(parlaylib)
if (NOT parlaylib_POPULATED)
    FetchContent_MakeAvailable(parlaylib)
endif ()
enable_testing()
find_package(benchmark CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_executable(perf perf.cpp
        bfs.h)
add_executable(correct correctness.cpp
        bfs.h)
target_link_libraries(perf PRIVATE parlay benchmark::benchmark benchmark::benchmark_main)
target_link_libraries(correct PRIVATE parlay GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
