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

#include "metric.hpp"

namespace analyser::metric::metric_impl
{
    struct CountParametersMetric final : public IMetric
    {
    protected:
        std::string Name() const final;
        MetricResult::ValueType CalculateImpl(const function::Function &f) const final;
    };
} // namespace analyser::metric::metric_impl
