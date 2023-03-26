#include <iostream>
#include <string>
#include <vector>
#include <charconv>
#include <algorithm>
#include <numeric>

int main() 
{
    uint32_t required_width = 0;
    uint32_t total_img_count = 0;
    uint32_t selected_img_count = 0;
    std::cin >> required_width;
    std::cin >> total_img_count;
    std::cin >> selected_img_count;

    std::string tmp;
    std::vector<uint32_t> widths(total_img_count);
    std::vector<uint32_t> heights(total_img_count);

    uint32_t cur_width = 0;
    uint32_t cur_height = 0;
    for (size_t i = 0; i < total_img_count; ++i) {
        std::cin >> tmp;

        auto width_end = tmp[3] == 'x' ? 3 : 4;
        auto from_char_res = std::from_chars(tmp.data(), tmp.data() + width_end, widths[i]);
        if (from_char_res.ec == std::errc::invalid_argument) {
            std::cout << 0 << std::endl << 0 << std::endl;
            return 1;
        }
        from_char_res = std::from_chars(tmp.data() + width_end + 1, tmp.data() + tmp.size() - width_end - 2, heights[i]);
        if (from_char_res.ec == std::errc::invalid_argument) {
            std::cout << 0 << std::endl << 0 << std::endl;
            return 1;
        }
    }

    for (size_t i = 0; i < total_img_count; ++i) {
        auto old_width = widths[i];
        // don't use old heights anymore, so update it inplace
        heights[i] = (heights[i] * required_width + old_width - 1)  / old_width;
    }
    std::sort(heights.begin(), heights.end());
    size_t min = std::accumulate(heights.begin(), heights.begin() + selected_img_count, 0);
    size_t max = std::accumulate(heights.rbegin(), heights.rbegin() + selected_img_count, 0);
    std:: cout << min << std::endl;
    std:: cout << max << std::endl;
	return 0;
}