#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl
{
    using Input = std::pair<std::string_view, MetricResult::ValueType>;
    static const std::filesystem::path testFilesDir = TEST_FILES_DIR;

    class CyclomatComplexMTest : public ::testing::TestWithParam<Input>{};
    TEST_P(CyclomatComplexMTest, TestByFilesList)
    {
        const auto &[file, expected] = GetParam();
        function::FunctionExtractor extractor;

        auto data = extractor.Get(file::File{testFilesDir / file});
        EXPECT_DOUBLE_EQ(CyclomaticComplexityMetric{}.Calculate(data.front()).value, expected);
    }

    INSTANTIATE_TEST_SUITE_P(
        TestByFilesList,
        CyclomatComplexMTest,
        ::testing::Values(
            Input{"comments.py", 1},
            Input{"if.py", 2},
            Input{"many_lines.py", 2},
            Input{"match_case.py", 5},
            Input{"simple.py", 2},
            Input{"exceptions.py", 5},
            Input{"loops.py", 4},
            Input{"many_parameters.py", 2},
            Input{"nested_if.py", 6},
            Input{"ternary.py", 3}));

} // namespace analyser::metric::metric_impl
