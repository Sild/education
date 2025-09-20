#pragma once
#include <map>
#include <vector>
#include <iostream>

struct Price {
    double value;
    bool operator<(const Price& other) const {
        return value < other.value;
    }
    bool operator>(const Price& other) const {
        return value > other.value;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Price& price) {
    os << price.value;
    return os;
}

struct OrderID {
    int id;
    static OrderID random() {
        static int current_id = 0;
        return OrderID{current_id++};
    }

    bool operator==(const OrderID& other) const {
        return id == other.id;
    }

};

template <> struct std::hash<OrderID> {
    size_t operator()(const OrderID& oid) const noexcept {
        return std::hash<int>{}(oid.id);
    }
};

enum OrderType {
    Bid,
    Ask,
};

struct Order {
    OrderType type;
    OrderID order_id;
    Price price;
    size_t quantity;

    static Order bid(const Price price, const size_t quantity) {
        return Order{Bid, OrderID::random(), price, quantity};
    }

    static Order ask(const Price price, const size_t quantity) {
        return Order{Ask, OrderID::random(), price, quantity};
    }
};

struct ExecutedOrder {
    OrderID order_id;
    Price price;
    size_t quantity;
    bool completed;
};

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

    void operator-=(const ExecutedOrder& order) {
        quantity -= order.quantity;
        if (order.completed) {
            count -= 1;
        }
    }
};



class OrderBook {
public:
    OrderBook() = default;
    ~OrderBook() = default;

    std::vector<ExecutedOrder> add(Order&& order);
    std::optional<Order> remove(const OrderID& order_id);
    void print() const;
private:
    std::map<Price, std::map<size_t, Order>, std::greater<>> bids;
    std::map<Price, OrderAggregation, std::greater<>> bids_aggregated;

    std::map<Price, std::map<size_t, Order>> asks;
    std::map<Price, OrderAggregation> asks_aggregated;

    size_t last_index = 0;
    std::unordered_map<OrderID, std::pair<size_t, Order*>> order_index;

    std::vector<ExecutedOrder> add_bid(Order&& order);
    std::vector<ExecutedOrder> add_ask(Order&& order);
};