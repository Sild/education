#pragma once
#include "order.h"
#include "order_fill.h"

struct OrderAggregation {
    size_t quantity;
    size_t count;

    void operator+=(const Order& order) {
        quantity += order.quantity;
        count += 1;
    }
    void operator-=(const Order& order) {
        quantity -= order.quantity;
        count -= 1;
    }

    friend std::ostream& operator<<(std::ostream& os, const OrderAggregation& agg) {
        os << "q: " << agg.quantity << ", c: " << agg.count;
        return os;
    }
};
