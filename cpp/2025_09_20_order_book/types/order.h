#pragma once
#include "order_id.h"
#include "price.h"

enum OrderType {
    Bid,
    Ask,
};

struct Order {
    OrderType type;
    OrderID id;
    Price price;
    size_t quantity;

    static Order bid(const Price price, const size_t quantity) {
        return Order{Bid, OrderID::random(), price, quantity};
    }

    static Order ask(const Price price, const size_t quantity) {
        return Order{Ask, OrderID::random(), price, quantity};
    }

    friend std::ostream& operator<<(std::ostream& os, const Order& order) {
        os
            << "t: " << (order.type == Bid ? "bid" : "ask")
            << ", id: " << order.id.id
            << ", p: " << order.price
            << ", q: " << order.quantity;
        return os;
    }
};