cmake_minimum_required(VERSION 3.8)

project(helloworld VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

configure_file(src/version.cpp.in ${PROJECT_SOURCE_DIR}/src/version.cpp)
configure_file(tests/test_version.cpp.in ${PROJECT_SOURCE_DIR}/tests/test_version.cpp)


INCLUDE_DIRECTORIES(
    src
    tests
)

add_executable(helloworld 
    src/main.cpp
    src/version.cpp
)

add_executable(test_version
    tests/test_version.cpp
    src/version.cpp
)

set_target_properties(helloworld
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

install(TARGETS helloworld RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")

set(CPACK_PACKAGE_CONTACT sildtm@icloud.com)

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)

enable_testing()

add_test(helloworld_tests
    test_version
)
