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
#include <print>

#include "analyse.hpp"
#include "cmd_options.hpp"
#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"
#include "metric_accumulator_impl/accumulators.hpp"
#include "metric_impl/metrics.hpp"

namespace metric_impl = analyser::metric::metric_impl;
namespace accamulator_impl = analyser::metric_accumulator::metric_accumulator_impl;
namespace accumulator_interface = analyser::metric_accumulator;

void PrinterForMetrics(accumulator_interface::MetricsAccumulator& accumulator)
{
    const auto paramCount = accumulator.GetFinalizedAccumulator<accamulator_impl::AverageAccumulator>("CountParametersMetric");
    const auto codeLines = accumulator.GetFinalizedAccumulator<accamulator_impl::SumAverageAccumulator>("CodeLinesCountMetric");
    const auto complexity = accumulator.GetFinalizedAccumulator<accamulator_impl::SumAverageAccumulator>("CyclomaticComplexityMetric");

    constexpr std::string_view offset("    ");
    std::print("{}CountParametersMetric_avg: {}\n", offset, paramCount.Get());
    std::print("{}CodeLinesCountMetric_sum: {}\n", offset, codeLines.Get().sum);
    std::print("{}CodeLinesCountMetric_avg: {}\n", offset, codeLines.Get().average);
    std::print("{}CyclomaticComplexityMetric_sum: {}\n", offset, complexity.Get().sum);
    std::print("{}CyclomaticComplexityMetric_avg: {}\n", offset, complexity.Get().average);
}

void PrinterForFiltered(std::string_view preview, auto& accum, const auto& unit)
{
    const auto &[name, analysis] = unit;
    std::print("{} {}\n", preview, name);
    analyser::AccumulateFunctionAnalysis(analysis, accum);
    PrinterForMetrics(accum);
}

int main(int argc, char *argv[])
{
    try
    {
        analyser::cmd::ProgramOptions options;
        if (!options.Parse(argc, argv))
            return 0;

        const std::vector<std::string> &inFiles = options.GetFiles();

        analyser::metric::MetricExtractor extractor;
        extractor.RegisterMetric(std::make_unique<metric_impl::CodeLinesCountMetric>());
        extractor.RegisterMetric(std::make_unique<metric_impl::CountParametersMetric>());
        extractor.RegisterMetric(std::make_unique<metric_impl::CyclomaticComplexityMetric>());

        auto analyse = analyser::AnalyseFunctions(inFiles, extractor);

        analyser::metric_accumulator::MetricsAccumulator accumulator;
        accumulator.RegisterAccumulator("CodeLinesCountMetric", std::make_unique<accamulator_impl::SumAverageAccumulator>());
        accumulator.RegisterAccumulator("CountParametersMetric", std::make_unique<accamulator_impl::AverageAccumulator>());
        accumulator.RegisterAccumulator("CyclomaticComplexityMetric", std::make_unique<accamulator_impl::SumAverageAccumulator>());

        std::ranges::for_each(analyse, [](const auto& unit)
        {
            const auto &[f, data] = unit;
            std::print("{}{}{}::{}:\n", f.filename, f.class_name ? "::" : "", f.class_name.value_or(""), f.name);
            std::ranges::for_each(data, [](const auto &val)
            { 
                std::print("    {}: {}\n", val.metric_name, val.value);
            });
        });

        std::ranges::for_each(analyser::SplitByFiles(analyse), [&accumulator](const auto& pair)
        {
            PrinterForFiltered("Accumulated Analysis for file", accumulator, pair);
        });

        std::ranges::for_each(analyser::SplitByClasses(analyse), [&accumulator](const auto& pair)
        {
            PrinterForFiltered("Accumulated Analysis for class", accumulator, pair);
        });

        std::print("Accumulated Analysis for all functions\n");
        analyser::AccumulateFunctionAnalysis(analyse, accumulator);
        PrinterForMetrics(accumulator);
    }
    catch (std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
