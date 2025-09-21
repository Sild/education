#include <cassert>

#include "../order_book.h"

int main() {
    OrderBook book;

    const auto bid_fills1 = book.add(Order::bid({11.0}, 5));
    assert(bid_fills1.empty());
    const auto aggr1 = book.levels_aggregation();
    assert(aggr1.size() == 1);
    assert(aggr1.begin()->second.quantity == 5);
    assert(aggr1.begin()->second.count == 1);

    const auto ask_fills1 = book.add(Order::ask({10.0}, 11));
    assert(ask_fills1.size() == 1);
    assert(ask_fills1[0].quantity == 5);
    assert(ask_fills1[0].bid_completed == true);
    assert(ask_fills1[0].ask_completed == false);
    const auto aggr2 = book.levels_aggregation();
    assert(aggr2.size() == 1);
    assert(aggr2.begin()->second.quantity == 6);
    assert(aggr2.begin()->second.count == 1);

    const auto bid_fills2 = book.add(Order::bid({11.0}, 6));
    assert(bid_fills2.size() == 1);
    assert(bid_fills2[0].quantity == 6);
    assert(bid_fills2[0].bid_completed == true);
    assert(bid_fills2[0].ask_completed == true);
    assert(book.levels_aggregation().empty());

    book.add(Order::bid({11.0}, 4));
    book.add(Order::bid({12.0}, 6));
    assert(book.levels_aggregation().size() == 2);

    auto ask_order3 = Order::ask({9.0}, 16);
    auto fills3 = book.add(Order(ask_order3));
    assert(fills3.size() == 2);
    assert(fills3[0].quantity == 6);
    assert(fills3[1].quantity == 4);

    auto aggr3 = book.levels_aggregation();
    assert(aggr3.size() == 1);
    assert(aggr3.begin()->second.quantity == 6);

    auto ask_order3_received = book.remove(ask_order3.id);
    assert(ask_order3_received.has_value());
    assert(ask_order3_received->quantity == 6);
    assert(book.levels_aggregation().empty());
    auto ask_order3_no_received = book.remove(ask_order3.id);
    assert(!ask_order3_no_received.has_value());
}

