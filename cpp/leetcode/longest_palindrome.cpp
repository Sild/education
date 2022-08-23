#include <string>
#include <iostream>

class Solution {
public:
    string longestPalindrome(string s) {
        if (s.size() < 2)
            return s;
        int left = 0;
        int right = 0;
        int max_left = 0;
        int max_right = 0;
        int cur = 1;
        for (int i = 1; i < s.size(); ++i) {
            left = i - 1;
            right = i + 1;
            cur = 1;
            while (left >= 0 && right < s.size() && s[left] == s[right]) {
                --left;
                ++right;
                ++cur;
            }
            if (cur > (max_right - max_left)) {
                max_left = left + 1;
                max_right = right - 1;
            }
            
            if (i + 1 < s.size() && s[i] == s[i+1]) {
                left = i - 1;
                right = i + 2;
                cur = 2;
                while (left >= 0 && right < s.size() && s[left] == s[right]) {
                    --left;
                    ++right;
                    ++cur;
                }
                if (cur > (max_right - max_left)) {
                    max_left = left + 1;
                    max_right = right - 1;
                }
            }
        }
        return s.substr(max_left, max_right + 1);
    }
private:
    int checkIndex(const std::string& s, int left, int right, int init) {
        // return polindrom length in this indexes
        while (left >= 0 && right < s.size() && s[left] == s[right]) {
            --left;
            ++right;
            init += 2;
        }
        return init;
    }
};

int main() {
}