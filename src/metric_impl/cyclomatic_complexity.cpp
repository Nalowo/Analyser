#include "metric_impl/cyclomatic_complexity.hpp"

#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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
    static const std::unordered_set<std::string_view> countWordsList
    {
        "if_statement",
        "elif_clause",
        "else_clause",
        "while_statement",
        "for_statement",
        "for_in_clause",
        "try_statement",
        "except_clause",
        "finally_clause",
        "match_statement",
        "case_clause",
        "assert_statement",
        "conditional_expression"
    };

    std::string CyclomaticComplexityMetric::Name() const
    {
        return "CyclomaticComplexityMetric";
    }

    MetricResult::ValueType CyclomaticComplexityMetric::CalculateImpl(const function::Function &f) const
    {
        static const std::regex regex{R"((\b[a-zA-Z_][a-zA-Z_0-9]*\b))"};
        MetricResult::ValueType res{0};
        std::string_view bodyAst = f.ast;
        if (auto pos = bodyAst.find("(block"); pos != std::string_view::npos)
            bodyAst = bodyAst.substr(pos);
        else return res;

        auto words = bodyAst | vs::split('\n') | vs::transform([&](auto line) 
        {
            using Iter = decltype(line.begin());
            std::regex_iterator<Iter> begin{ line.begin(), line.end(), regex };
            std::regex_iterator<Iter> end{};
            return rs::subrange{begin, end}; 
        }) | vs::join | vs::join;
        
        res = rs::count_if(words, [](const auto& w)
        {
            std::string_view str{std::to_address(w.first), static_cast<size_t>(w.length())};
            if (countWordsList.contains(str))
                return true;
            return false;
        }) + 1;

        return res;
    }

} // namespace analyser::metric::metric_impl
