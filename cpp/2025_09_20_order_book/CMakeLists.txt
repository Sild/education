cmake_minimum_required(VERSION 4.0)
project(2025_09_20_order_book)

set(CMAKE_CXX_STANDARD 20)

add_executable(2025_09_20_order_book
        main.cpp
        order_book.h
        order_book.cpp
        types/order_id.h
        types/order.h
)
