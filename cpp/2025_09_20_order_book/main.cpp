#include <iostream>
#include "order_book.h"

int main() {
    OrderBook book;

    for (int i = 0; i < 10; ++i) {
        book.add(Order::bid({100.0 + i}, 10 + i));
        book.add(Order::ask({101.0 + i}, 5 + i));
    }
    book.print();
    return 0;
}