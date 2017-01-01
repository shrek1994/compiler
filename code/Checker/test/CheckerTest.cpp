#include <Checker.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

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
        in.str("");
        out.str("");
        info.str("");
        error.str("");
    }

    std::stringstream in, out, info, error, expected;
    std::shared_ptr<Checker> checker = std::make_shared<Checker>(out);
};

TEST_F(CheckerTest, shouldCheckEmptyCodeCorrectly)
{
    std::vector<std::string> vars = {};
    in << "VAR BEGIN SKIP; END\n";
    expected << "BEGIN SKIP; END\n";

    checker->run(in);

    EXPECT_STREQ(in.str().c_str(), out.str().c_str());
    ASSERT_VECTOR(vars, checker->getVariables());
}

}