cmake_minimum_required(VERSION 3.5)

project(bayan VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS program_options filesystem regex thread REQUIRED)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_FLAGS "-std=c++17")
set(CMAKE_COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")
set(CMAKE_CXX_COMPILER "clang++")
#set(CMAKE_CXX_FLAGS
#        "-fsanitize=thread -g -O1"
#        "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O1"
#        CACHE STRING "Flags used by the C++ compiler during ThreadSanitizer builds."
#        FORCE)

INCLUDE_DIRECTORIES(
    src
)


add_executable(bayan
    src/main.cpp
)

set_target_properties(bayan
        PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/src"
)

TARGET_LINK_LIBRARIES( bayan LINK_PUBLIC ${Boost_LIBRARIES} pthread)




install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")

set(CPACK_PACKAGE_CONTACT sildtm@icloud.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
