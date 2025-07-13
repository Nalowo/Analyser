#include "metric_impl/code_lines_count.hpp"
#include "utils.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>
#include <unordered_set>
#include <regex>

namespace rv = std::ranges::views;
namespace rs = std::ranges;
namespace vs = std::views;

namespace analyser::metric::metric_impl
{
    MetricResult::ValueType CodeLinesCountMetric::CalculateImpl(const function::Function &f) const
    {
        static const std::regex regex{R"((?:\()(\w+)(?: *)(?:\[)(\d+)(?:.*)(?:\[)(\d+))"};
        auto all_matches = f.ast 
                                | vs::split('\n') 
                                | vs::transform([&](auto line) 
                                {
                                    using Iter = decltype(line.begin());
                                    std::regex_iterator<Iter> begin{ line.begin(), line.end(), regex };
                                    std::regex_iterator<Iter> end{};
                                    return std::ranges::subrange{begin, end}; 
                                })
                                | vs::join
                                | vs::filter([](const std::smatch& m) 
                                {
                                    std::string_view str{std::to_address(m[1].first), static_cast<size_t>(m[1].length()) };
                                    // есть идеи как фильтровать string, но не уверен что это нужно
                                    return str != "comment";
                                })
                                | vs::transform([](const std::smatch& match)
                                {
                                    return std::array{ToInt(match[2].str()), ToInt(match[3].str())};
                                })
                                | vs::join;
        return static_cast<MetricResult::ValueType>(std::unordered_set<int>(std::from_range, all_matches).size());
    }
    std::string CodeLinesCountMetric::Name() const
    {
        using namespace std::string_literals;
        return "CodeLinesCountMetric"s;
    }

} // namespace analyser::metric::metric_impl
