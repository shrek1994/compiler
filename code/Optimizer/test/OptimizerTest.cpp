#include <Optimizer.hpp>
#include <debug.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace optimizer {


class OptimizerTest : public ::testing::Test
{
public:
    OptimizerTest() = default;
    virtual ~OptimizerTest() = default;

    void SetUp() override
    {
        Logger::enableDebug = false;
        in.str("");
        out.str("");
        info.str("");
        error.str("");
    }

    std::stringstream in, out, info, error, expected;
    std::shared_ptr<Optimizer> optimizer = std::make_shared<Optimizer>(out);
};


TEST_F(OptimizerTest, shouldOptimizeEmptyCode)
{
    expected << "BEGIN SKIP; END\n";
    optimizer->run(expected);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(OptimizerTest, DISABLED_shouldCorrectReadVars)
{
    in << "VAR a          b\nc test[2]\tdBEGIN SKIP; END\n";
    expected << "BEGIN SKIP; END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
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