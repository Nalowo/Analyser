#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyser
{

    namespace rv = std::ranges::views;
    namespace rs = std::ranges;
    namespace vs = std::views;

    using namespace analyser::metric;
    using namespace analyser::file;
    using namespace analyser::function;
    using namespace analyser::metric_accumulator;

    auto AnalyseFunctions(const std::vector<std::string> &files,
                          const MetricExtractor &metric_extractor)
    {
        std::vector<std::pair<Function, MetricResults>> result;
        FunctionExtractor fExtractor;

        auto functionsList = files | vs::transform([&fExtractor](const auto &fileName)
                                                   { return fExtractor.Get(File(fileName)); }) |
                             vs::join | vs::transform([&metric_extractor](const auto &f)
                                                      { return std::make_pair(f, metric_extractor.Get(f)); });
        result.reserve(rs::distance(functionsList));
        rs::copy(functionsList, std::back_inserter(result));
        return result;
    }

    auto SplitByClasses(const auto &analysis)
    {
        // здесь ваш код
    }

    auto SplitByFiles(const auto &analysis)
    {
        // здесь ваш код
    }

    void AccumulateFunctionAnalysis(const auto &analysis,
                                    const MetricsAccumulator &accumulator)
    {
        // здесь ваш код
    }

} // namespace analyser
