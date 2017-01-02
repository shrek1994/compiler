#include <Checker.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "debug.hpp"

#define ASSERT_VECTOR(expected, actual) \
    ASSERT_EQ(expected.size(), actual.size()); \
    for (int i=0; i < expected.size(); ++i) \
        EXPECT_EQ(expected[i], actual[i]) << "i = " << i <<", \'" \
            << expected[i] << "\' != \'" << actual[i] << "\'"

namespace checker {

class CheckerTest : public ::testing::Test
{
public:
    CheckerTest() = default;
    virtual ~CheckerTest() = default;

    void SetUp() override
    {
        Logger::enableDebug = false;
        in.str("");
        out.str("");
        info.str("");
        error.str("");
    }

    std::stringstream in, out, info, error, expected;
    std::shared_ptr<Checker> checker = std::make_shared<Checker>(out, info, error);
};

TEST_F(CheckerTest, shouldCheckEmptyCodeCorrectly)
{
    in << "VAR BEGIN SKIP; END\n";
    expected << "BEGIN\nSKIP;\nEND\n";

    checker->run(in);

    EXPECT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(CheckerTest, shouldCorrectReadVariable)
{
    std::vector<std::string> vars = { "a", "b" };
    in << "VAR\ta\nb BEGIN SKIP; END\n";
    expected << "BEGIN\nSKIP;\nEND\n";

    checker->run(in);

    EXPECT_STREQ(expected.str().c_str(), out.str().c_str());
    ASSERT_VECTOR(vars, checker->getVariables());
}

TEST_F(CheckerTest, shouldCorrectReadTabs)
{
    std::vector<std::string> vars = { "a", "b", "abc", "abc", "abc", "abc"};
    in << "VAR\ta\nb abc[4] BEGIN SKIP; END\n";
    expected << "BEGIN\nSKIP;\nEND\n";

    checker->run(in);

    EXPECT_STREQ(expected.str().c_str(), out.str().c_str());
    ASSERT_VECTOR(vars, checker->getVariables());
}

TEST_F(CheckerTest, shouldCorrectRemoveComments)
{
    std::vector<std::string> vars = { "a", "b", "abc", "abc", "abc", "abc"};
    in << "{sat}VAR {sad}\t{saet} a {stas\nate}\nb {ast} {BEGIN WRITE 5; END} abc[4] "
            "BEGIN {WRITE 4;}SKIP; END{WRITE 5;}\n";
    expected << "BEGIN\nSKIP;\nEND\n";

    checker->run(in);

    EXPECT_STREQ(expected.str().c_str(), out.str().c_str());
    ASSERT_VECTOR(vars, checker->getVariables());
}


TEST_F(CheckerTest, shouldShowWarningAndRepairMissingSemicolonInAssign)
{
    auto warning = Checker::warning + "4" + Checker::missingSemicolon + "\n";
    std::vector<std::string> vars = { "a", "b" };
    in << "VAR\\\n"
            "a b\n"
            "BEGIN\n"
            "a := b\n"
            "END\n";
    expected << "BEGIN\na := b;\nEND\n";

    checker->run(in);

    EXPECT_STREQ(expected.str().c_str(), out.str().c_str());
    EXPECT_STREQ(warning.c_str(), info.str().c_str());
    ASSERT_VECTOR(vars, checker->getVariables());
}

}
