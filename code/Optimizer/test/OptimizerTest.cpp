#include <Optimizer.hpp>
#include <debug.hpp>
#include <Generator/inc/Driver.hpp>
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

    std::stringstream in, out, info, error, expected, null;
    std::shared_ptr<checker::Checker> check = std::make_shared<checker::Checker>(null, null, null);
    std::shared_ptr<Optimizer> optimizer = std::make_shared<Optimizer>(out, check);
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

TEST_F(OptimizerTest, shouldChangeFor_upTo)
{
    in <<
       "BEGIN "
           "FOR i FROM a TO b DO\n"
               "WRITE i;\n"
           "ENDFOR\n"
       "END\n";
    expected <<
             "BEGIN\n"
                 "i := a;\n"
                 "FOR0 := b;\n"
                 "%FOR0%: $reg1 := i - FOR0;\n"
                 "JZERO 1 %ELSE0%;\n"
                 "SKIP;\n"
                 "JUMP %ENDIF0%;\n"
                 "%ELSE0%: WRITE i;\n"
                 "i := i + 1;\n"
                 "JUMP %FOR0%;\n"
                 "%ENDIF0%: "
             "END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(OptimizerTest, shouldChangeFor_upDownto)
{
    in <<
        "BEGIN "
           "FOR i FROM a DOWNTO b DO\n"
           "WRITE i;\n"
           "ENDFOR\n"
        "END\n";
    expected <<
        "BEGIN\n"
             "i := a;\n"
             "%FOR0%: $reg1 := b - i;\n"
             "JZERO 1 %ELSE0%;\n"
             "SKIP;\n"
             "JUMP %ENDIF0%;\n"
             "%ELSE0%: WRITE i;\n"
             "i := i - 1;\n"
             "JUMP %FOR0%;\n"
             "%ENDIF0%: "
        "END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(OptimizerTest, shouldConvertMul)
{
    in <<
       "BEGIN "
       "a := 1;\n"
       "b := 2;\n"
       "c := a * b;\n"
       "WRITE c;\n"
       "END\n";
    expected <<
        "BEGIN\n"
            "a := 1;\n"
            "b := 2;\n"
            "ZERO 1;\n"
            "$reg2 := a;\n"
            "$reg3 := b;\n"
            "%MUL0%: JZERO 3 %ENDMUL0%;\n"
            "JODD 3 %ADD0%;\n"
            "JUMP %MULSKIP0%;\n"
            "%ADD0%: " + jftt::varTemp.name + " := $reg2;\n"
            "ADD 1;\n"
            "%MULSKIP0%: SHL 2;\n"
            "SHR 3;\n"
            "JUMP %MUL0%;\n"
            "%ENDMUL0%: c := $reg1;\n"
            "WRITE c;\n"
        "END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

TEST_F(OptimizerTest, shouldConvertDiv)
{
    in <<
       "BEGIN "
       "a := 74;\n"
       "b := 6;\n"
       "c := a / b;\n"
       "WRITE c;\n"
       "END\n";
    expected <<
             "BEGIN\n"
                     "a := 74;\n"
                     "b := 6;\n"
                     "ZERO 1;\n"
                     "$reg2 := a;\n"
                     "$reg3 := b;\n"

                     //add:
                     "%DIVWHILE0%: "+ jftt::varTemp.name + " := $reg3;\n"
                     "$reg4 := a - " + jftt::varTemp.name +  ";\n"
                     "JZERO 4 %ENDDIVWHILE0%;\n"
                     "SHL 3;\n"
                     "JUMP %DIVWHILE0%;\n"

                     //div:
                     "%ENDDIVWHILE0%: "
                    + jftt::varTemp.name + " := $reg3;\n"
                     "$reg4 := b - " + jftt::varTemp.name + ";\n"
                     "JZERO 4 %DIV0%;\n"
                     "JUMP %ENDDIV0%;\n"
                     "%DIV0%: SHL 1;\n"
                    + jftt::varTemp.name + " := $reg3;\n"
                     "LOAD 4;\n"
                     "STORE 2;\n"
                     "SUB 4;\n"

                     "JZERO 4 %PERFORMDIV0%;\n"
                     "JUMP %SKIPDIV0%;\n"
                     "%PERFORMDIV0%: INC 1;\n"
                    + jftt::varTemp.name + " := $reg3;\n"
                     "SUB 2;\n"


                     "%SKIPDIV0%: SHR 3;\n"
                     "JUMP %ENDDIVWHILE0%;\n"
                     "%ENDDIV0%: c := $reg1;\n"

                     "WRITE c;\n"
             "END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}


TEST_F(OptimizerTest, shouldConvertModulo)
{
    in <<
       "BEGIN "
               "a := 74;\n"
               "b := 6;\n"
               "c := a % b;\n"
               "WRITE c;\n"
               "END\n";
    expected <<
             "BEGIN\n"
                     "a := 74;\n"
                     "b := 6;\n"
                     "ZERO 1;\n"
                     "$reg2 := a;\n"
                     "$reg3 := b;\n"

                     //add:
                     "%DIVWHILE0%: "+ jftt::varTemp.name + " := $reg3;\n"
                     "$reg4 := a - " + jftt::varTemp.name +  ";\n"
                     "JZERO 4 %ENDDIVWHILE0%;\n"
                     "SHL 3;\n"
                     "JUMP %DIVWHILE0%;\n"

                     //div:
                     "%ENDDIVWHILE0%: "
             + jftt::varTemp.name + " := $reg3;\n"
                     "$reg4 := b - " + jftt::varTemp.name + ";\n"
                     "JZERO 4 %DIV0%;\n"
                     "JUMP %ENDDIV0%;\n"
                     "%DIV0%: SHL 1;\n"
             + jftt::varTemp.name + " := $reg3;\n"
                     "LOAD 4;\n"
                     "STORE 2;\n"
                     "SUB 4;\n"

                     "JZERO 4 %PERFORMDIV0%;\n"
                     "JUMP %SKIPDIV0%;\n"
                     "%PERFORMDIV0%: INC 1;\n"
             + jftt::varTemp.name + " := $reg3;\n"
                     "SUB 2;\n"


                     "%SKIPDIV0%: SHR 3;\n"
                     "JUMP %ENDDIVWHILE0%;\n"
                     "%ENDDIV0%: c := $reg2;\n"

                     "WRITE c;\n"
                     "END\n";
    optimizer->run(in);
    ASSERT_STREQ(expected.str().c_str(), out.str().c_str());
}

}