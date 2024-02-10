cmake_minimum_required(VERSION 3.5)

project(matrix_multi VERSION 1.0)

# find_package(Boost COMPONENTS program_options filesystem regex thread REQUIRED)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_FLAGS "-std=c++20")
set(CMAKE_COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Wno-everything")
set(CMAKE_CXX_FLAGS "-O2 -march=native -w") # -unroll-count=4")
set(CMAKE_CXX_COMPILER "clang++")
INCLUDE_DIRECTORIES(
    src
    /usr/local/include/eigen3/
)

add_executable(m_multi
    src/main.cpp
)

find_package(benchmark REQUIRED)

add_executable(m_multi_perf
    src/main_perf.cpp
)

target_link_libraries(m_multi_perf
    benchmark::benchmark
    # fmt
)

# enable_testing()
# add_test(tests
#     iteration_perf_test
# )
