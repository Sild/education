cmake_minimum_required(VERSION 3.5)

project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)
set(COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")

INCLUDE_DIRECTORIES(
    src
)

add_executable(allocator 
    src/main.cpp
)

add_executable(allocator_tests
    tests/allocator_tests.cpp
)

set_target_properties(allocator_tests
    PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/src"
)

add_library(custom_allocator src/allocator.cpp)
add_library(custom_container src/container.cpp)

target_link_libraries(allocator custom_allocator custom_container)
target_link_libraries(allocator_tests custom_allocator custom_container)


install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")

set(CPACK_PACKAGE_CONTACT sildtm@icloud.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()

add_test(tests
    allocator_tests
)
