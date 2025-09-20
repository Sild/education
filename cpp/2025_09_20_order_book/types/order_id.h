#pragma once

struct OrderID {
    int id;
    bool operator==(const OrderID& other) const {
        return id == other.id;
    }

    static OrderID random() {
        static int current_id = 0;
        return OrderID{current_id++};
    }

};

template <> struct std::hash<OrderID> {
    size_t operator()(const OrderID& oid) const noexcept {
        return std::hash<int>{}(oid.id);
    }
};