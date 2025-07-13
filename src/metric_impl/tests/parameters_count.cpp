#include "metric_impl/parameters_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl
{

    using Input = std::pair<std::string_view, MetricResult::ValueType>;
    static const std::filesystem::path testFilesDir = TEST_FILES_DIR;

    class ParamCountMTest : public ::testing::TestWithParam<Input>{};
    TEST_P(ParamCountMTest, TestByFilesList)
    {
        const auto &[file, expected] = GetParam();
        function::FunctionExtractor extractor;

        auto data = extractor.Get(file::File{testFilesDir / file});
        EXPECT_DOUBLE_EQ(CountParametersMetric{}.Calculate(data.front()).value, expected);
    }

    INSTANTIATE_TEST_SUITE_P(
        TestByFilesList,
        ParamCountMTest,
        ::testing::Values(
        Input{"comments.py", 3},
        Input{"if.py", 1},
        Input{"many_lines.py", 0},
        Input{"match_case.py", 1},
        Input{"simple.py", 0},
        Input{"exceptions.py", 0},
        Input{"loops.py", 1},
        Input{"many_parameters.py", 5},
        Input{"nested_if.py", 2},
        Input{"ternary.py", 1}));

} // namespace analyser::metric::metric_impl
