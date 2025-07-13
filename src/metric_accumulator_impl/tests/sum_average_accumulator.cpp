#include "metric_accumulator_impl/sum_average_accumulator.hpp"
#include <gtest/gtest.h>

namespace analyser::metric_accumulator::metric_accumulator_impl::test
{

    TEST(SumAverageAccumulatorTest, EmptyTest)
    {
        SumAverageAccumulator accumulator;
        accumulator.Finalize();
        EXPECT_TRUE(accumulator.Get() == SumAverageAccumulator::SumAverage(0, 0));
    }

    TEST(SumAverageAccumulatorTest, NotFinalizedTest)
    {
        SumAverageAccumulator accumulator;
        ASSERT_EQ(accumulator.IsFinalized(), false);
        EXPECT_THROW(accumulator.Get(), std::runtime_error);
    }

    TEST(SumAverageAccumulatorTest, TryAccumulateFinalizedTest)
    {
        SumAverageAccumulator accumulator;
        accumulator.Finalize();
        ASSERT_EQ(accumulator.IsFinalized(), true);
        EXPECT_THROW(accumulator.Accumulate(metric::MetricResult{"", 0}), std::runtime_error);
    }

    TEST(SumAverageAccumulatorTest, InputTest)
    {
        SumAverageAccumulator accumulator;
        accumulator.Accumulate(metric::MetricResult{"", 2});
        accumulator.Accumulate(metric::MetricResult{"", 40});
        accumulator.Finalize();
        EXPECT_TRUE(accumulator.Get() == SumAverageAccumulator::SumAverage(42, 21));
    }

    TEST(SumAverageAccumulatorTest, ResetAndAccumulateTest)
    {
        SumAverageAccumulator accumulator;
        accumulator.Accumulate(metric::MetricResult{"", 40});
        accumulator.Accumulate(metric::MetricResult{"", 2});
        accumulator.Finalize();
        EXPECT_TRUE(accumulator.Get() == SumAverageAccumulator::SumAverage(42, 21));
        accumulator.Reset();
        accumulator.Accumulate(metric::MetricResult{"", 33});
        accumulator.Accumulate(metric::MetricResult{"", 7});
        accumulator.Accumulate(metric::MetricResult{"", 2});
        accumulator.Finalize();
        EXPECT_TRUE(accumulator.Get() == SumAverageAccumulator::SumAverage(42, 14));
    }
} // namespace analyser::metric_accumulator::metric_accumulator_impl::test
