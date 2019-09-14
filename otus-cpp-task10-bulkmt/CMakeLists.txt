cmake_minimum_required(VERSION 3.5)

project(bulkmt VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_FLAGS -std=c++17)
set(CMAKE_COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")
set(CMAKE_CXX_COMPILER "clang++")

#set(CMAKE_CXX_FLAGS
#        "-fsanitize=thread -g -O1")
#        "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O1"
#        CACHE STRING "Flags used by the C++ compiler during ThreadSanitizer builds."
#        FORCE)

INCLUDE_DIRECTORIES(
    src
)

add_executable(bulkmt
    src/main.cpp
)

add_library(input_reader
        src/input_reader.cpp
)

add_library(data_handler
        src/data_handler.cpp
)

target_link_libraries(bulkmt
        input_reader
        data_handler
        pthread
)

install(TARGETS bulkmt RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")

set(CPACK_PACKAGE_CONTACT sildtm@icloud.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)