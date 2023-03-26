cmake_minimum_required(VERSION 3.5)

project(life VERSION 1.0)

# find_package(Boost COMPONENTS program_options filesystem regex thread REQUIRED)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_FLAGS "-std=c++20")
set(CMAKE_COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")
set(CMAKE_CXX_COMPILER "clang++")

add_executable(life
    src/main.cpp
)

set(CPACK_PACKAGE_CONTACT sildtm@icloud.com)

