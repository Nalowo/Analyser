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
#include <unordered_map>

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

    using AnalysisUnit = std::pair<function::Function, metric::MetricResults>;
    using AnalysisResults = std::vector<std::pair<function::Function, metric::MetricResults>>;
    using refVector = std::vector<std::reference_wrapper<const AnalysisUnit>>;

    auto AnalyseFunctions(const std::vector<std::string> &files,
                          const MetricExtractor &metric_extractor)
    {
        AnalysisResults result;
        FunctionExtractor fExtractor;

        auto functionsList = files | vs::transform([&fExtractor](const auto &fileName)
        { 
            return fExtractor.Get(File(fileName)); 
        }) | vs::join | vs::transform([&metric_extractor](const auto &f)
        { 
            return std::make_pair(f, metric_extractor.Get(f)); 
        });
        result.reserve(rs::distance(functionsList));
        rs::copy(functionsList, std::back_inserter(result));
        return result;
    }

    auto SplitByClasses(const AnalysisResults &analysis)
    {
        std::vector<std::pair<std::string, refVector>> result;
        std::unordered_map<std::string_view, std::reference_wrapper<refVector>> map;

        auto vv = analysis | vs::filter([](const auto& val)
        {
            const auto& f = val.first;
            return f.class_name.has_value() && !f.class_name->empty();;
        }) | vs::transform([&result, &map](const auto& val)
        {
            std::string_view name = val.first.class_name.value();
            if (auto it = map.find(name); it == map.end())
            {
                auto& pair = result.emplace_back(std::pair{std::string(name), refVector{std::ref(val)}});
                map.emplace(name, std::ref(pair.second));
            }
            else
                it->second.get().push_back(std::ref(val));
            return true;
        });
        
        rs::for_each(vv, [](bool){});
        return result;
    }

    auto SplitByFiles(const AnalysisResults &analysis)
    {
        std::vector<std::pair<std::string, refVector>> result;
        std::unordered_map<std::string_view, std::reference_wrapper<refVector>> map;

        auto vv = analysis | vs::transform([&result, &map](const auto& val)
        {
            std::string_view name = val.first.filename;
            if (auto it = map.find(name); it == map.end())
            {
                auto& pair = result.emplace_back(std::pair{std::string(name), refVector{std::ref(val)}});
                map.emplace(name, std::ref(pair.second));
            }
            else
                it->second.get().push_back(std::ref(val));
            return true;
        });
        
        rs::for_each(vv, [](bool){});
        return result;
    }

    template <typename T>
    void AccumulateFunctionAnalysis(const T& analysis,
                                    MetricsAccumulator &accumulator)
    {
        static_assert(false && "No implement");
    }

    template <>
    void AccumulateFunctionAnalysis(const refVector &analysis,
                                    MetricsAccumulator &accumulator)
    {
        accumulator.ResetAccumulators();
        rs::for_each(analysis, [&accumulator](const auto &group_ref)
        { 
            const auto &group = group_ref.get(); 
            accumulator.AccumulateNextFunctionResults(group.second); 
        });
    }

    template <>
    void AccumulateFunctionAnalysis(const AnalysisResults& analysis,
                                    MetricsAccumulator &accumulator)
    {
        accumulator.ResetAccumulators();
        rs::for_each(analysis | vs::values, [&accumulator](const auto& m) 
        {
            accumulator.AccumulateNextFunctionResults(m);
        });
    }
} // namespace analyser
