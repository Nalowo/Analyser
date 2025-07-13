#include "metric_accumulator_impl/average_accumulator.hpp"
#include <gtest/gtest.h>

namespace analyser::metric_accumulator::metric_accumulator_impl::test
{

    TEST(AverageAccumulatorTest, EmptyTest)
    {
        AverageAccumulator accumulator;
        accumulator.Finalize();
        EXPECT_DOUBLE_EQ(accumulator.Get(), 0);
    }

    TEST(AverageAccumulatorTest, NotFinalizedTest)
    {
        AverageAccumulator accumulator;
        ASSERT_EQ(accumulator.IsFinalized(), false);
        EXPECT_THROW(accumulator.Get(), std::runtime_error);
    }

    TEST(AverageAccumulatorTest, TryAccumulateFinalizedTest)
    {
        AverageAccumulator accumulator;
        accumulator.Finalize();
        ASSERT_EQ(accumulator.IsFinalized(), true);
        EXPECT_THROW(accumulator.Accumulate(metric::MetricResult{"", 0}), std::runtime_error);
    }

    TEST(AverageAccumulatorTest, InputTest)
    {
        AverageAccumulator accumulator;
        accumulator.Accumulate(metric::MetricResult{"", 6});
        accumulator.Accumulate(metric::MetricResult{"", 3});
        accumulator.Finalize();
        EXPECT_DOUBLE_EQ(accumulator.Get(), 4.5);
    }

    TEST(AverageAccumulatorTest, ResetAndAccumulateTest)
    {
        AverageAccumulator accumulator;
        accumulator.Accumulate(metric::MetricResult{"", 3});
        accumulator.Accumulate(metric::MetricResult{"", 6});
        accumulator.Finalize();
        EXPECT_DOUBLE_EQ(accumulator.Get(), 4.5);
        accumulator.Reset();
        accumulator.Accumulate(metric::MetricResult{"", 5});
        accumulator.Accumulate(metric::MetricResult{"", 3});
        accumulator.Accumulate(metric::MetricResult{"", 10});
        accumulator.Finalize();
        EXPECT_DOUBLE_EQ(accumulator.Get(), 6);
    }

} // namespace analyser::metric_accumulator::metric_accumulator_impl::test
