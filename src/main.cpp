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

int main(int argc, char *argv[])
{
    analyser::cmd::ProgramOptions options;
    if (!options.Parse(argc, argv))
        return 0;

    const std::vector<std::string> &inFiles = options.GetFiles();

    analyser::metric::MetricExtractor extractor;
    extractor.RegisterMetric(std::make_unique<metric_impl::CodeLinesCountMetric>());
    extractor.RegisterMetric( std::make_unique<metric_impl::CountParametersMetric>() );
    extractor.RegisterMetric( std::make_unique<metric_impl::CyclomaticComplexityMetric>() );
    // extractor.RegisterMetric( std::make_unique<metric_impl::NamingStyleMetric>() );

    auto analyse = analyser::AnalyseFunctions(inFiles, extractor);

    analyser::metric_accumulator::MetricsAccumulator accumulator;
    accumulator.RegisterAccumulator( "CodeLinesCountMetric", std::make_unique<accamulator_impl::SumAverageAccumulator>() );
    accumulator.RegisterAccumulator( "CyclomaticComplexityMetric", std::make_unique<accamulator_impl::SumAverageAccumulator>() );
    accumulator.RegisterAccumulator( "CountParametersMetric", std::make_unique<accamulator_impl::AverageAccumulator>() );
    // зарегистрируйте аккумуляторы метрик в accumulator

    // запустите analyser::SplitByFiles
    // запустите analyser::AccumulateFunctionAnalysis для каждого подмножества результатов метрик
    // выведете результаты на консоль

    // запустите analyser::SplitByClasses
    // запустите analyser::AccumulateFunctionAnalysis для каждого подмножества результатов метрик
    // выведете результаты на консоль

    // запустите analyser::AccumulateFunctionAnalysis для всех результатов метрик
    // выведете результаты на консоль

    return 0;
}
