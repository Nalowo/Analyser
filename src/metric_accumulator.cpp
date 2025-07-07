#include "metric_accumulator.hpp"

#include <unistd.h>

#include <algorithm>
#include <any>
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
#include <print>

namespace analyser::metric_accumulator
{

    void MetricsAccumulator::AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const
    {
        std::ranges::for_each(metric_results, [this](const auto &result)
        {
            std::string name{result.metric_name};
            if (const auto it = _accumulators.find(name); it != _accumulators.end())
                it->second->Accumulate(result);
            else
                throw std::runtime_error(std::format("Metric {} not found", name));
        });
    }

    void MetricsAccumulator::ResetAccumulators()
    {
        std::ranges::for_each(_accumulators | std::views::values, [](const auto &i) 
        {
            if (i) i->Reset();
        });
    }

} // namespace analyser::metric_accumulator
