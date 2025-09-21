cmake_minimum_required(VERSION 4.0)
project(order_book)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

add_library(order_book_lib
        order_book.cpp
        order_book.h
        types/order_id.h
        types/order.h
        types/price.h
        types/order_fill.h
        types/order_aggregation.h
)


add_executable(order_book_example
        main.cpp
)
target_link_libraries(order_book_example PRIVATE order_book_lib)

add_executable(order_book_tests
        tests/order_book_tests.cpp
)
target_link_libraries(order_book_tests PRIVATE order_book_lib)

add_test(
    NAME order_book_test
    COMMAND order_book_tests
)
