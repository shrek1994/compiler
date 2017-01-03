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
    in << "BEGIN SKIP; END\n";
    expected << "BEGIN\nSKIP;\nEND\n";
    optimizer->run(expected);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(OptimizerTest, assign)
{
    in << "BEGIN a := 5; b := 2; END\n";
    expected << "BEGIN\na := 5;\nb := 2;\nEND\n";
    optimizer->run(expected);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(OptimizerTest, shouldChangeIf_BiggerThan)
{
    in <<
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
             "BEGIN\n"
                 "a := 1;\n"
                 "b := 2;\n"
                 "$reg1 := a - b;\n"
                 "JZERO 1 %ELSE0%;\n"
                 "WRITE a;\n"
                 "JUMP %ENDIF0%;\n"
                 "%ELSE0%: WRITE b;\n"
                 "%ENDIF0%: WRITE 5;\n"
             "END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(OptimizerTest, shouldChangeIf_LowerThan)
{
    in <<
       "BEGIN "
               "a := 1; "
               "b := 2; "
               "IF a < b THEN "
               "WRITE a; "
               "ELSE "
               "WRITE b; "
               "ENDIF "
               "WRITE 5;"
               "END\n";
    expected <<
             "BEGIN\n"
                     "a := 1;\n"
                     "b := 2;\n"
                     "$reg1 := b - a;\n"
                     "JZERO 1 %ELSE0%;\n"
                     "WRITE a;\n"
                     "JUMP %ENDIF0%;\n"
                     "%ELSE0%: WRITE b;\n"
                     "%ENDIF0%: WRITE 5;\n"
                     "END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(OptimizerTest, shouldChangeIf_LowerOrEqThan)
{
    in <<
       "BEGIN "
           "a := 1; "
           "b := 2; "
           "IF a <= b THEN "
           "WRITE a; "
           "ELSE "
           "WRITE b; "
           "ENDIF "
           "WRITE 5;"
       "END\n";
    expected <<
             "BEGIN\n"
                 "a := 1;\n"
                 "b := 2;\n"
                 "$reg1 := a - b;\n"
                 "JZERO 1 %ELSE0%;\n"
                 "WRITE b;\n"
                 "JUMP %ENDIF0%;\n"
                 "%ELSE0%: WRITE a;\n"
                 "%ENDIF0%: WRITE 5;\n"
             "END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(OptimizerTest, shouldChangeIf_BiggerOrEqThan)
{
    in <<
       "BEGIN "
           "a := 1; "
           "b := 2; "
           "IF a >= b THEN "
           "WRITE a; "
           "ELSE "
           "WRITE b; "
           "ENDIF "
           "WRITE 5;"
       "END\n";
    expected <<
             "BEGIN\n"
                 "a := 1;\n"
                 "b := 2;\n"
                 "$reg1 := b - a;\n"
                 "JZERO 1 %ELSE0%;\n"
                 "WRITE b;\n"
                 "JUMP %ENDIF0%;\n"
                 "%ELSE0%: WRITE a;\n"
                 "%ENDIF0%: WRITE 5;\n"
             "END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(OptimizerTest, shouldChangeIf_Eq)
{
    in <<
       "BEGIN "
               "a := 1; "
               "b := 2; "
               "IF a = b THEN "
               "WRITE a; "
               "ELSE "
               "WRITE b; "
               "ENDIF "
               "WRITE 5;"
       "END\n";
    expected <<
             "BEGIN\n"
                 "a := 1;\n"
                 "b := 2;\n"
                 "$reg1 := a - b;\n"
                 "$reg2 := b - a;\n"
                 "JZERO 1 %NEXT0%;\n"
                 "JUMP %ELSE0%;\n"
                 "%NEXT0%: JZERO 2 %IF0%;\n"
                 "%ELSE0%: WRITE b;\n"
                 "JUMP %ENDIF0%;\n"
                 "%IF0%: WRITE a;\n"
                 "%ENDIF0%: WRITE 5;\n"
             "END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(OptimizerTest, shouldChangeIf_NotEq)
{
    in <<
       "BEGIN "
               "a := 1; "
               "b := 2; "
               "IF a <> b THEN "
               "WRITE a; "
               "ELSE "
               "WRITE b; "
               "ENDIF "
               "WRITE 5;"
               "END\n";
    expected <<
             "BEGIN\n"
                     "a := 1;\n"
                     "b := 2;\n"
                     "$reg1 := a - b;\n"
                     "$reg2 := b - a;\n"
                     "JZERO 1 %NEXT0%;\n"
                     "JUMP %ELSE0%;\n"
                     "%NEXT0%: JZERO 2 %IF0%;\n"
                     "%ELSE0%: WRITE a;\n"
                     "JUMP %ENDIF0%;\n"
                     "%IF0%: WRITE b;\n"
                     "%ENDIF0%: WRITE 5;\n"
                     "END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(OptimizerTest, shouldChangeWhile_LowerThan)
{
    in <<
       "BEGIN "
               "a := 1; "
               "b := 5; "
               "WHILE a < b DO "
               "WRITE a; "
               "a := a + 1; "
               "ENDWHILE "
               "WRITE 5;"
       "END\n";
    expected <<
             "BEGIN\n"
                 "a := 1;\n"
                 "b := 5;\n"
                 "%WHILE0%: "
                    "$reg1 := b - a;\n"
                 "JZERO 1 %ELSE0%;\n"
                 "WRITE a;\n"
                 "a := a + 1;\n"
                 "JUMP %WHILE0%;\n"
                 "JUMP %ENDIF0%;\n"
                 "%ELSE0%: "
                    "SKIP;\n"
                 "%ENDIF0%: "
                     "WRITE 5;\n"
             "END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}



}