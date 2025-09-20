#include "order_book.h"

#include <iostream>
#include <ostream>
#include <ranges>

void OrderBook::print() const {
    std::cout << "===asks===" << std::endl;
    for (auto& [price, agg] : asks_aggregated | std::views::reverse) {
        std::cout << price << ": " << agg.quantity << " (" << agg.count << ")\n";
    }

    std::cout << "===bids===" << std::endl;
    for (auto& [price, agg]: this->bids_aggregated) {
        std::cout << "price: " << price << ", q: " << agg.quantity << ", c: " << agg.count << std::endl;
    }
}

std::vector<ExecutedOrder> OrderBook::add(Order &&order) {
    switch (order.type) {
        case Bid:
            return this->add_bid(std::move(order));
        case Ask:
            return this->add_ask(std::move(order));
    }
}


std::vector<ExecutedOrder> OrderBook::add_bid(Order&& order) {

    while (auto& [price, asks] = this->asks.begin)




    this->bids_aggregated[order.price] += order;
    auto& map = this->bids[order.price];
    map.emplace_hint(map.end(), this->last_index++, std::move(order));
    return {}; // TODO
}

std::vector<ExecutedOrder> OrderBook::add_ask(Order&& order) {
    this->asks_aggregated[order.price] += order;
    auto& map = this->asks[order.price];
    map.emplace_hint(map.end(), this->last_index++, std::move(order));
    return {}; // TODO
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
            return this->bids[price].extract(index).mapped();
        case Ask:
            this->asks_aggregated[price] -= *order_ptr;
            return this->asks[price].extract(index).mapped();
    }
}
