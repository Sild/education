cmake_minimum_required(VERSION 3.5)

project(async VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
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

add_executable(async_cli
    src/main.cpp
)

add_library(async SHARED 
        src/async.cpp 
        src/async.h
        src/data_handler.cpp
        src/input_reader.cpp
)

target_link_libraries(async_cli
        pthread
        async
)

install(TARGETS async
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        COMPONENT library
)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")

set(CPACK_PACKAGE_CONTACT sildtm@icloud.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)