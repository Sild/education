#pragma once
#include "order_id.h"
#include "price.h"

struct OrderFill {
    OrderID bid_id;
    OrderID ask_id;
    bool bid_completed;
    bool ask_completed;
    Price price;
    size_t quantity;

    // stdout
    friend std::ostream& operator<<(std::ostream& os, const OrderFill& fill) {
        os
            << "bid_id " << fill.bid_id.id
            << ", ask_id: " << fill.ask_id.id
            << ", p: " << fill.price
            << ", q: " << fill.quantity
            << ", bid_comp: " << fill.bid_completed
            << ", ask_comp: " << fill.ask_completed;
        return os;
    }



};
