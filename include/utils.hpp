#pragma once

#include <charconv>
#include <stdexcept>
#include <string>
#include <string_view>
#include <ranges>
#include <print>

inline int ToInt(std::string_view value)
{
    int result{};
    auto [parse_end_ptr, error_code] = std::from_chars(value.begin(), value.end(), result);
    if (error_code != std::errc{} || parse_end_ptr != value.data() + value.size())
    {
        throw std::invalid_argument("Cannot convert '" + std::string(value) + "' to integral");
    }
    return result;
}

// inline std::string_view GetBlock(std::string_view str)
// {
//     std::string_view res;
//     size_t pos = 1;
//     auto parametersView = str | std::views::drop(1) | std::views::take_while([&pos](char c)
//     {
//         c == '(' ? ++pos : (c == ')' ? --pos : 0);
//         return pos > 0; 
//     });

//     res = std::string_view{std::to_address(parametersView.begin()), static_cast<std::string_view::size_type>(std::ranges::distance(parametersView))};
//     if (pos > 0)
//         throw std::runtime_error(std::format("Invalid block: {}", str.substr(0, str.find_first_of('\n'))));
//     return res;
// }