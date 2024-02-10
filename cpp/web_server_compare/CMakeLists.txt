cmake_minimum_required(VERSION 3.8)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_REQUIRED_FLAGS -std=c++20)

set(CMAKE_CXX_COMPILER /usr/bin/clang++)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(HPX_WITH_MALLOC "system")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
# find_package(TCMalloc REQUIRED)

include_directories(
    external
    external/libsniper
)
set(RTBTECH_DEPS "http")

add_subdirectory(external)

add_executable(server
    src/main.cpp
)

set(PROJECT_LIBS
        ${CMAKE_THREAD_LIBS_INIT}
        ${BOOST_LIBRARIES}
        backtrace dl
        stdc++fs.a
        tcmalloc
        )

target_link_libraries(server ${SNIPER_LIBRARIES} ${PROJECT_LIBS})
link_directories(/usr/local/lib)

# target_link_libraries(server
#     lib-asio
#     lib-beast
# )
