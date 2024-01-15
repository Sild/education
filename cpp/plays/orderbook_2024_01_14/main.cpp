#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace my_ns {

using OrderID = int;

struct Order {
    OrderID id = 0;
    int ts = 0;
    std::string symbol;
    double price = 0.0;
    double quantity = 0.0;
    bool ask = false;
};

std::istream& operator>>(std::istream& input, Order& dst) {
    static auto parse_property = [](std::istream& input, auto& dst) {
        input >> dst;
        input.ignore();  // skip comma
    };

    parse_property(input, dst.id);
    parse_property(input, dst.ts);
    std::getline(input, dst.symbol, ',');  // parse to string is a bit tricky
    parse_property(input, dst.price);
    parse_property(input, dst.quantity);
    dst.ask = input.peek() == 's';
    return input;
}

std::ostream& operator<<(std::ostream& output, const Order& obj) {
    output << "id: " << obj.id << " ts: " << obj.ts << " symbol: " << obj.symbol << " price: " << obj.price
           << " quantity: " << obj.quantity << " ask: " << obj.ask;
    return output;
}

std::vector<Order> parse_input() {
    // assuming input is correct
    std::vector<Order> orders;
    std::string line;
    std::string side;
    std::getline(std::cin, line);  // skip header

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        orders.emplace_back();
        iss >> orders.back();
    }
    return orders;
}

bool eq_zero(double x) { return std::abs(x) < 1e-9; }

struct Deal {
    OrderID bid_id = 0;
    OrderID ask_id = 0;
    double sum = 0.0;
    double quantity = 0.0;
};

std::ostream& operator<<(std::ostream& output, const Deal& obj) {
    output << "bid_id: " << obj.bid_id << " ask_id: " << obj.ask_id << " sum: " << obj.sum << " q: " << obj.quantity;
    return output;
}

class SymOrderBook {
    // assume ts is unique
    std::map<double, std::map<int, Order*>, std::greater<>> bids;  // price -> ts -> order
    std::map<double, std::map<int, Order*>, std::less<>> asks;
    std::unordered_map<OrderID, Order> orders_store;

   public:
    std::vector<Deal> upsert_order(Order&& order) {
        if (auto it = orders_store.find(order.id); it != orders_store.end()) {
            std::cerr << "first\n";
            remove_order(order.id);
        }
        // no match - fill the book
        if (order.ask && (bids.empty() || bids.begin()->first < order.price)) {
            std::cerr << "aks check\n";
            if (!bids.empty()) {
                std::cerr << "bid: " << bids.begin()->first << "\n";
            }
            insert_order(std::move(order));
            orders_store[order.id] = std::move(order);
            return {};
        }
        if (!order.ask && (asks.empty() || asks.begin()->first > order.price)) {
            std::cerr << "bid check\n";
            insert_order(std::move(order));
            return {};
        }

        std::vector<Deal> deals;
        // match is guaranteed already
        if (order.ask) {
            while (true) {
                auto it = bids.begin();
                if (it == bids.end() || it->first < order.price) {
                    break;
                }
                for (auto order_it = it->second.begin(); order_it != it->second.end();) {
                    auto deal = make_deal(*(order_it->second), order, it->first);
                    deals.emplace_back(std::move(deal));
                    if (eq_zero(order_it->second->quantity)) {
                        orders_store.erase(order_it->second->id);
                        order_it = it->second.erase(order_it);
                        if (it->second.empty()) {
                            asks.erase(it);
                        }
                    }
                    if (eq_zero(order.quantity)) {
                        return deals;
                    }
                }
            }
        } else {
            while (true) {
                auto it = asks.begin();
                if (it == asks.end() || it->first > order.price) {
                    break;
                }
                for (auto order_it = it->second.begin(); order_it != it->second.end();) {
                    auto deal = make_deal(order, *(order_it->second), it->first);
                    deals.emplace_back(std::move(deal));
                    if (eq_zero(order_it->second->quantity)) {
                        orders_store.erase(order_it->second->id);
                        order_it = it->second.erase(order_it);
                        if (it->second.empty()) {
                            asks.erase(it);
                        }
                    }
                    if (eq_zero(order.quantity)) {
                        return deals;
                    }
                }
            }
        }
        insert_order(std::move(order));

        return deals;
    }

    void insert_order(Order&& order) {
        const auto& [it, _] = orders_store.emplace(order.id, std::move(order));
        if (order.ask) {
            asks[it->second.price].emplace(it->second.ts, &(it->second));
        } else {
            bids[it->second.price].emplace(it->second.ts, &(it->second));
        }
    }

    void remove_order(OrderID order_id) {
        // expect state is valid and order exists
        auto order_it = orders_store.find(order_id);
        if (order_it->second.ask) {
            asks[order_it->second.price].erase(order_it->second.ts);
        } else {
            bids[order_it->second.price].erase(order_it->second.ts);
        }
        orders_store.erase(order_it);
    }

   private:
    Deal make_deal(Order& bid_order, Order& ask_order, double price) {
        // expect bid >= ask
        double deal_q = std::min(bid_order.quantity, ask_order.quantity);
        double deal_sum = deal_q * price;
        bid_order.quantity -= deal_q;
        ask_order.quantity -= deal_q;
        return Deal{.bid_id = bid_order.id, .ask_id = ask_order.id, .sum = deal_sum, .quantity = deal_q};
    }
};

}  // namespace my_ns

using namespace my_ns;
int main() {
    std::vector<Order> orders;
    orders = parse_input();
    try {
    } catch (std::exception& e) {
        std::cerr << "input error: " << e.what() << "\n";
        return 1;
    }

    std::cout << "orders size: " << orders.size() << "\n";
    auto book = SymOrderBook();
    for (auto& o : orders) {
        std::cout << "add orderer: " << o << "\n";
        auto deals = book.upsert_order(std::move(o));
        std::cout << "deals size: " << deals.size() << "\n";
        for (auto& d : deals) {
            std::cout << "deal: " << d << "\n";
        }
    }
    orders.clear();
}
