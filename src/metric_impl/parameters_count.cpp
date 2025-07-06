#include "metric_impl/parameters_count.hpp"

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

namespace rv = std::ranges::views;
namespace rs = std::ranges;
namespace vs = std::views;

namespace analyser::metric::metric_impl
{
    MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function &f) const
    {
        MetricResult::ValueType res{0};
        std::string_view astView = f.ast;
        if (auto pos = astView.find("(parameters"); pos != std::string::npos)
            astView = astView.substr(pos);
        else return res;

        astView = [](std::string_view str)
        {
            size_t pos = 1;
            auto parametersView = str | vs::drop(1) | vs::take_while([&pos](char c)
            {
                c == '(' ? ++pos : (c == ')' ? --pos : 0);
                return pos > 0; 
            });
            return std::string_view{std::to_address(parametersView.begin()), static_cast<std::string_view::size_type>(rs::distance(parametersView))};
        }(astView);

        res = [](std::string_view str)
        {
            size_t pos = 0, counter = 0;
            rs::for_each(str, [&](char c)
            {
                c == '(' ? (pos == 0 ? ++counter : 0), ++pos : (c == ')' ? --pos : 0);
            });
            return counter;
        }(astView);
        return res;
    }

    std::string CountParametersMetric::Name() const
    {
        return "ParametersCountMetric";
    }

} // namespace analyser::metric::metric_impl
