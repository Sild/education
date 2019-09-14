cmake_minimum_required(VERSION 3.5)

project(join_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_FLAGS -std=c++17)
set(CMAKE_COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-fcoroutines-ts --stdlib=libc++ -Xclang -fconcepts-ts -lpthread")

# assume built-in pthreads on MacOS
IF(APPLE)
    set(CMAKE_THREAD_LIBS_INIT "-lpthread")
    set(CMAKE_HAVE_THREADS_LIBRARY 1)
    set(CMAKE_USE_WIN32_THREADS_INIT 0)
    set(CMAKE_USE_PTHREADS_INIT 1)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
ENDIF()

find_package(Boost 1.71.0 REQUIRED system)


#set(CMAKE_CXX_FLAGS
#        "-fsanitize=thread -g -O1")
#        "-fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O1"
#        CACHE STRING "Flags used by the C++ compiler during ThreadSanitizer builds."
#        FORCE)


add_executable(join_server
        src/main.cpp
        src/db.cpp
)

target_include_directories(join_server
        PRIVATE 
        src
        ${Boost_INCLUDE_DIRS}
)

install(TARGETS join_server
        RUNTIME DESTINATION bin
)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "$ENV{TRAVIS_BUILD_NUMBER}")

set(CPACK_PACKAGE_CONTACT sildtm@icloud.com)

# set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)