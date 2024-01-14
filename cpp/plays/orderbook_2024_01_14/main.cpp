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
    dst.ask = input.peek() == 'b';
    return input;
}

std::ostream& operator<<(std::ostream& output, const Order& obj) {
    output << "ts: " << obj.ts << " symbol: " << obj.symbol << " price: " << obj.price << " quantity: " << obj.quantity
           << " buy: " << obj.ask;
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

bool eq_zero(double x) {
    return std::abs(x) < 1e-9;
}

struct Deal {
    OrderID bid_id = 0;
    OrderID ask_id = 0;
    double sum = 0.0;
    double quantity = 0.0;
};

class SymOrderBook {
    std::map<double, Order, std::less<>> bids;
    std::map<double, Order, std::greater<>> asks;
    std::unordered_map<OrderID, Order> orders_store;

   public:
    std::vector<Deal> upsert_order(Order& order) {
        // no match - fill the book
        if (order.ask && (bids.empty() || bids.begin()->first > order.price)) {
            orders_store[order.id] = order;  // TODO it's not enough
            return {};
        }
        if (!order.ask && (asks.empty() || asks.begin()->first < order.price)) {
            orders_store[order.id] = order;  // TODO it's not enough
            return {};
        }

        std::vector<Deal> deals;
        // match is guaranteed already
        if (order.ask) {
            while (true) {
                auto it = bids.begin();
                if (it->first < order.price) {
                    break;
                }
                auto deal = make_deal(it->second, order, it->first); // make a loop
                deals.emplace_back(std::move(deal));
                if (eq_zero(it->second.quantity)) {
                    remove_order(it->second.id);
                }
                if (eq_zero(order.quantity)) {
                    return deals;
                }
            }
        }
    }

    void remove_order(OrderID id) {
        auto it = orders_store.find(id);

        if (it == orders_store.end()) {
            return;
        }
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
    for (const auto& e : orders) {
        std::cout << e << "\n";
    }
}
