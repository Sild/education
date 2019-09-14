cmake_minimum_required(VERSION 3.5)

project(bulk VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_FLAGS -std=c++17)
set(CMAKE_COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")
set(CMAKE_CXX_COMPILER "clang++")

INCLUDE_DIRECTORIES(
    src
)

add_executable(bulk
    src/main.cpp
)

add_library(input_reader
        src/input_reader.cpp
)

add_library(data_handler
        src/data_handler.cpp
)

target_link_libraries(bulk
        input_reader
        data_handler
)

install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")

set(CPACK_PACKAGE_CONTACT sildtm@icloud.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)