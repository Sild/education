#include <algorithm>
#include <assert.h>
#include <string>
#include <array>
#include <iostream>

using namespace std;

class Solution {
public:
    int lengthOfLongestSubstring(const string& s) {
        std::cout << "str=" << s << std::endl;
        std::array<int, 256> char_pos;
        char_pos.fill(-1);

        if (s.empty())
            return 0;

        int max = 0;
        int cur_len = 0;
        int i = 0;
        int start_pos = 0;
        while (i < s.size())
        {  
            char ch = s[i];
            // std::cout << "i=" << i << ", char=" << ch << std::endl;
            if (const auto dup_pos = char_pos[ch]; dup_pos >= start_pos && i != dup_pos)
            {
                max = std::max(max, cur_len);
                char_pos[ch] = -1;
                i = dup_pos + 1;
                start_pos = i;
                cur_len = 0;
                // std::cout << "max=" << max << ", dup_pos=" << dup_pos << ", new_i=" << i << std::endl;
            }
            else
            {
                char_pos[ch] = i;
                ++cur_len;
                ++i;
            }
            // std::cout << "cur_len=" << cur_len << std::endl;
            
        }
        
        return std::max(max, cur_len);
    }
};

int main()
{
    Solution sol;
    assert(sol.lengthOfLongestSubstring("tmmzuxt") == 5);

    assert(sol.lengthOfLongestSubstring("abcabcbb") == 3);
    assert(sol.lengthOfLongestSubstring("abcabfgt") == 6);
    assert(sol.lengthOfLongestSubstring("au") == 2);
    assert(sol.lengthOfLongestSubstring("cdd") == 2);
    assert(sol.lengthOfLongestSubstring("abba") == 2);
}