#pragma once

#include <iostream>

struct Price {
    double value;
    bool operator<(const Price& other) const { return value < other.value; }
    bool operator <=(const Price& other) const { return value <= other.value; }
    bool operator >=(const Price& other) const { return value >= other.value; }
    bool operator>(const Price& other) const { return value > other.value; }
    [[nodiscard]] bool is_zero() const { return value == 0.0; }

};

inline std::ostream& operator<<(std::ostream& os, const Price& price) {
    os << price.value;
    return os;
}