#pragma once
#include <map>
#include <vector>
#include <iostream>

#include "types/order_fill.h"
#include "types/order.h"
#include "types/order_aggregation.h"


class OrderBook {
public:
    OrderBook() = default;
    ~OrderBook() = default;

    std::vector<OrderFill> add(Order&& order);
    std::optional<Order> remove(const OrderID& order_id);
    std::map<Price, OrderAggregation> levels_aggregation();
    void print() const;
private:
    std::map<Price, std::map<size_t, Order>, std::greater<>> bids;
    std::map<Price, OrderAggregation> bids_aggregated;

    std::map<Price, std::map<size_t, Order>> asks;
    std::map<Price, OrderAggregation> asks_aggregated;

    size_t last_index = 0;
    std::unordered_map<OrderID, std::pair<size_t, Order*>> order_index;

    // Cares about aggregation as well
    std::vector<OrderFill> try_exec_order(Order& order);
};
