cmake_minimum_required(VERSION 3.5)

project(m1_matrix VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_FLAGS "-std=c++20")
set(CMAKE_COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")
set(CMAKE_CXX_COMPILER "clang++")


find_package(benchmark REQUIRED)

add_executable(m1_matrix
    main.cpp
)

target_link_libraries(m1_matrix benchmark::benchmark)

# enable_testing()
# add_test(tests
#     iteration_perf_test
# )
