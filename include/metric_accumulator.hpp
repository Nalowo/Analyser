#pragma once
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

#include "metric.hpp"

namespace rv = std::ranges::views;
namespace rs = std::ranges;

namespace analyser::metric_accumulator {

struct IAccumulator {
    virtual void Accumulate(const metric::MetricResult &metric_result) = 0;
    virtual void Finalize() = 0;
    virtual void Reset() = 0;
    virtual ~IAccumulator() = default;

protected:
    bool is_finalized = false;
};

struct MetricsAccumulator {
    template <typename Accumulator>
    requires std::is_base_of_v<IAccumulator, Accumulator>
    void RegisterAccumulator(const std::string &metric_name, std::unique_ptr<Accumulator> acc) {
        if (auto [it, status] = _accumulators.emplace(metric_name, std::move(acc)); !status) 
            throw std::runtime_error("Accumulator: " + metric_name + " already exists");
    }

    template <typename Accumulator>
    requires std::is_base_of_v<IAccumulator, Accumulator>
    const Accumulator &GetFinalizedAccumulator(const std::string &metric_name) const {
        auto it = _accumulators.find(metric_name);
        if (it == _accumulators.end())
            throw std::runtime_error("Accumulator: " + metric_name + " does not exist");

        auto& derived = std::dynamic_pointer_cast<Accumulator>(it->second); 
        if (!derived->is_finalized)
            derived->Finalize();

        return *derived;
    }
    void AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const;
    void ResetAccumulators();

private:
    std::unordered_map<std::string, std::shared_ptr<IAccumulator>> _accumulators;
};

}  // namespace analyser::metric_accumulator
