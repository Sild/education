#include "order_book.h"

#include <assert.h>
#include <iostream>
#include <ostream>
#include <ranges>

// template <typename Book>
// concept BookSide = requires(Book b, Price p) {
//     // book[p] gives a std::map<size_t, Order>&
//     { b[p] } -> std::same_as<std::map<size_t, Order>&>;
// };
//
// template <typename Aggregation>
// concept AggSide = requires(Aggregation a, Price p, Order o) {
//     { a[p] += o };
// };


void OrderBook::print() const {
    std::cout << "===asks===" << std::endl;
    for (auto& [price, agg] : asks_aggregated | std::views::reverse) {
        std::cout << "price: " << price << ", " << agg << std::endl;
    }

    std::cout << "===bids===" << std::endl;
    for (auto& [price, agg]: this->bids_aggregated | std::views::reverse) {
        std::cout << "price: " << price << ", q: " << agg.quantity << ", c: " << agg.count << std::endl;
    }
}

template <class SideLevels, class SideAggr, class OrderIndex>
static void add_order_impl(SideLevels& levels, SideAggr& aggr, Order&& order, size_t& last_index, OrderIndex& order_index) {
    aggr[order.price] += order;
    auto order_id = order.id;
    auto& level = levels[order.price];
    level.emplace_hint(level.end(), last_index++, std::move(order));
    order_index.emplace(order_id, std::make_pair(last_index - 1, &level.rbegin()->second));
}

std::vector<OrderFill> OrderBook::add(Order &&order) {
    auto res = this->try_exec_order(order);
    if (order.quantity == 0) {
        return res;
    }
    switch (order.type) {
        case Bid:
            add_order_impl(this->bids, this->bids_aggregated, std::move(order), this->last_index, this->order_index);
            break;
        case Ask:
            add_order_impl(this->asks, this->asks_aggregated, std::move(order), this->last_index, this->order_index);
            break;
    }
    return res;
}

std::optional<Order> OrderBook::remove(const OrderID& order_id) {
    const auto extracted = this->order_index.extract(order_id);
    if (extracted.empty()) {
        return std::nullopt;
    }
    auto& [index, order_ptr] = extracted.mapped();
    const auto price = order_ptr->price;
    switch (order_ptr->type) {
        case Bid:
            this->bids_aggregated[price] -= *order_ptr;
            if (this->bids_aggregated[price].count == 0) {
                this->bids_aggregated.erase(price);
            }
            return this->bids[price].extract(index).mapped();
        case Ask:
            this->asks_aggregated[price] -= *order_ptr;
            if (this->asks_aggregated[price].count == 0) {
                this->asks_aggregated.erase(price);
            }
            return this->asks[price].extract(index).mapped();
    }
}

std::map<Price, OrderAggregation> OrderBook::levels_aggregation() {
    auto res = this->asks_aggregated;
    res.insert(this->bids_aggregated.begin(), this->bids_aggregated.end());
    return res;
}

static bool price_match(const Order& order, const Price& level_price) {
    switch (order.type) {
        case Bid:
            return order.price >= level_price;
        case Ask:
            return order.price <= level_price;
    }
}


template <class Book, class Aggregation, class Index>
static std::vector<OrderFill> try_exec_order_impl(Book& book, Aggregation& aggregation, Index& index, Order& order) {
    auto fills = std::vector<OrderFill>{};

    auto level_it = book.begin();

    while (level_it != book.end() && order.quantity > 0 && price_match(order, level_it->first)) {
        auto& [level_price, level_orders] = *level_it;
        auto level_order_it = level_orders.begin();
        while (level_order_it != level_orders.end() && order.quantity > 0) {
            auto& level_order = level_order_it->second;
            const size_t exec_quantity = std::min(order.quantity, level_order.quantity);

            order.quantity -= exec_quantity;
            level_order.quantity -= exec_quantity;

            const auto level_order_completed = level_order.quantity == 0;

            auto bid_id = order.type == Bid ? order.id : level_order.id;
            auto ask_id = order.type == Ask ? order.id : level_order.id;
            auto bid_completed = order.type == Bid ? order.quantity == 0 : level_order_completed;
            auto ask_completed = order.type == Ask ? order.quantity == 0 : level_order_completed;

            auto fill = OrderFill{
                .bid_id = bid_id,
                .ask_id = ask_id,
                .bid_completed = bid_completed,
                .ask_completed = ask_completed,
                .price = level_price,
                .quantity = exec_quantity,
            };

            fills.emplace_back(std::move(fill));

            // update aggregations
            auto& level_agg = aggregation[level_price];
            level_agg.quantity -= exec_quantity;

            // cleanup index
            if (level_order_completed) {
                if (--level_agg.count == 0) {;
                    aggregation.erase(level_price);
                }
                index.erase(level_order.id);
                level_orders.erase(level_order_it);
                if (level_orders.empty()) {
                    level_it = book.erase(level_it);
                    break; // Move to next level
                }
                ++level_order_it;
            }
            // else - order is completed and we'll exit the loop
        }
        if (level_order_it == level_orders.end()) {
            ++level_it;
        }
    }
    return fills;
}

std::vector<OrderFill> OrderBook::try_exec_order(Order &order) {
    if (order.quantity == 0) {
        return {};
    }

    switch (order.type) {
        case Bid:
            return try_exec_order_impl(this->asks, this->asks_aggregated, this->order_index, order);
        case Ask:
            return try_exec_order_impl(this->bids, this->bids_aggregated, this->order_index, order);
    }
}
