#include <Optimizer.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#define ASSERT_VECTOR(expected, actual) \
    ASSERT_EQ(expected.size(), actual.size()); \
    for (int i=0; i < expected.size(); ++i) \
        EXPECT_EQ(expected[i], actual[i]) << "i = " << i <<", \'" \
            << expected[i] << "\' != \'" << actual[i] << "\'"


namespace optimizer {


class OptimizerTest : public ::testing::Test
{
public:
    OptimizerTest() = default;
    virtual ~OptimizerTest() = default;

    void SetUp() override
    {
        in.str("");
        out.str("");
        info.str("");
        error.str("");
    }

    std::stringstream in, out, info, error, expected;
    std::shared_ptr<Optimizer> optimizer = std::make_shared<Optimizer>(out);
};


TEST_F(OptimizerTest, DISABLED_shouldCheckEmptyCodeCorrectly)
{
    in << "VAR BEGIN SKIP; END\n";
    expected << "BEGIN SKIP; END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(OptimizerTest, DISABLED_shouldCorrectReadVars)
{
    std::vector<std::string> vars = { "a", "b", "c", "test", "test" };
    in << "VAR a          b\nc test[2]\tdBEGIN SKIP; END\n";
    expected << "BEGIN SKIP; END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
    ASSERT_VECTOR(vars, optimizer->getVariables());
}

TEST_F(OptimizerTest, DISABLED_shouldChangeIf)
{
    in << "VAR a b "
        "BEGIN "
            "a := 1; "
            "b := 2; "
            "IF a > b THEN "
                "WRITE a; "
            "ELSE "
                "WRITE b; "
            "ENDIF "
            "WRITE 5;"
        "END\n";
    expected <<
             "VAR "
                "a b "
             "BEGIN "
                 "a := 1; "
                 "b := 2; "
                 "$reg1 := a - b; "
                 "JZERO 1 %ELSE0%; "
                 "%IF0%: WRITE a; "
                 "JUMP %ENDIF0%; "
                 "%ELSE%: WRITE b; "
                 "%ENDIF0%: "
                 "WRITE 5;"
             "END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

}