#include <iostream>
#include "order_book.h"

int main() {
    OrderBook book;

    for (int i = 0; i < 10; ++i) {
        auto bid_order = Order::bid({101.0 + i}, 5 + i);
        auto bid_copy = bid_order;
        const auto bid_fills = book.add(std::move(bid_order));
        std::cout << "added bid[" << i << "] (" << bid_copy << "), filled: " << bid_fills.size() << std::endl;
        for (const auto& fill : bid_fills) {
            std::cout << "fill: " << fill << std::endl;
        }

        auto ask_order = Order::ask({100.0 + i}, 10 + i);
        auto ask_copy = ask_order;
        auto ask_fills = book.add(std::move(ask_order));
        std::cout << "added ask[" << i << "] (" << ask_copy << "): filled: " << ask_fills.size() << std::endl;
        for (const auto& fill : ask_fills) {
            std::cout << "fill: " << fill << std::endl;
        }
    }
    book.print();
    return 0;
}