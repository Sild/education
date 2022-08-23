#include <iostream>
#include <string>
#include <limits>

int myAtoi(std::string s) {
        size_t sNumCount;
        bool sNegative = false;
        bool sBegin = true;
        int64_t res = 0;
        auto max = std::numeric_limits<int>::max();
        auto min = std::numeric_limits<int>::min();
        for (auto c: s)
        {
            if (c == ' ')
            {
                if (!sBegin)
                    break;
                else
                {
                    continue;
                }
            }
            
            if (c == '-' || c == '+')
            {
                if (!sBegin)
                    break;
                sBegin = false;
                if (c == '-')
                    sNegative = true;
                continue;
            }
            if (c >= '0' && c <= '9')
            {
                res *= 10;
                res -= (c - '0');
                if (res < min)
                {
                    res = min;
                    break;
                }
                continue;
            }
            break;
        }
        if (!sNegative)
        {
            res *= -1;
            if (res > max)
                res = max;
        }
        return res;
}


int main() {
    std::cout << myAtoi("42") << std::endl;
    std::cout << myAtoi("   -42") << std::endl;
}