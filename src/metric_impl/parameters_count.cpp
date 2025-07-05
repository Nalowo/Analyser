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

namespace analyser::metric::metric_impl {

MetricResult::ValueType CountParametersMetric::CalculateImpl(const function::Function &f) const
{
    std::string_view astView = f.ast;
    auto pos = astView.find("(parameters");
    if (pos == std::string::npos)
        return static_cast<MetricResult::ValueType>(0);
    astView = astView.substr(pos);

    pos = 1;
    auto parametersView = astView | std::views::drop(1) | std::views::take_while([&pos](char c)
    {
        if (pos < 1)
            return false;
        c == '(' ? ++pos : (c == ')' ? --pos : 0);
        return true;
    });


    std::ranges::for_each(parametersView, [](char c)
    {
        std::cout << c;
    });
    std::cout << '\n';
    int stop = 0;
    return 0;
}

std::string CountParametersMetric::Name() const
{ 
    return "ParametersCount";
}

}  // namespace analyser::metric::metric_impl
