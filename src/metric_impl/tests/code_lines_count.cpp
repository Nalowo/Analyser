#include "metric_impl/code_lines_count.hpp"

#include <gtest/gtest.h>

namespace analyser::metric::metric_impl
{
    using Input = std::pair<std::string_view, MetricResult::ValueType>;
    static const std::filesystem::path testFilesDir = TEST_FILES_DIR;

    class CodeLineCountMTest : public ::testing::TestWithParam<Input> {};
    TEST_P(CodeLineCountMTest, TestByFilesList)
    {
        const auto &[file, expected] = GetParam();
        function::FunctionExtractor extractor;

        auto data = extractor.Get(file::File{testFilesDir / file});
        EXPECT_DOUBLE_EQ(CodeLinesCountMetric{}.Calculate(data.front()).value, expected);
    }

    INSTANTIATE_TEST_SUITE_P(
        TestByFilesList,
        CodeLineCountMTest,
        ::testing::Values(
            Input{"comments.py", 4},
            Input{"if.py", 4},
            Input{"many_lines.py", 12},
            Input{"match_case.py", 8},
            Input{"simple.py", 6},
            Input{"exceptions.py", 8},
            Input{"loops.py", 7},
            Input{"many_parameters.py", 2},
            Input{"nested_if.py", 9},
            Input{"ternary.py", 2}));

} // namespace analyser::metric::metric_impl
