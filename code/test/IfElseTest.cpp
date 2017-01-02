#include "CompilerTest.hpp"

namespace {
std::string emptyIn = "";

const std::vector<Param> params = {
        Param{"VAR a b BEGIN a := 1; b := 2; "
                      "IF a < b THEN "
                      "WRITE 1; "
                      "ELSE "
                      "WRITE 2; "
                      "ENDIF "
                      "END\n",
              emptyIn, "> 1\n", "shouldCorrectPerformIfCondition_IfLower"},
        Param{"VAR a b BEGIN a := 2; b := 2; "
                      "IF a < b THEN "
                      "WRITE 1; "
                      "ELSE "
                      "WRITE 2; "
                      "ENDIF "
                      "END\n",
              emptyIn, "> 2\n", "shouldCorrectPerformIfCondition_ElseLower"},

        Param{"VAR a b BEGIN a := 3; b := 2; "
                      "IF a > b THEN "
                      "WRITE 1; "
                      "ELSE "
                      "WRITE 2; "
                      "ENDIF "
                      "END\n",
              emptyIn, "> 1\n", "shouldCorrectPerformIfCondition_IfBigger"},
        Param{"VAR a b BEGIN a := 2; b := 2; "
                      "IF a > b THEN "
                      "WRITE 1; "
                      "ELSE "
                      "WRITE 2; "
                      "ENDIF "
                      "END\n",
              emptyIn, "> 2\n", "shouldCorrectPerformIfCondition_ElseBigger"},

        Param{"VAR a b BEGIN a := 2; b := 2; "
                      "IF a >= b THEN "
                      "WRITE 1; "
                      "ELSE "
                      "WRITE 2; "
                      "ENDIF "
                      "END\n",
              emptyIn, "> 1\n", "shouldCorrectPerformIfCondition_IfBiggerOrEq"},
        Param{"VAR a b BEGIN a := 1; b := 2; "
                      "IF a >= b THEN "
                      "WRITE 1; "
                      "ELSE "
                      "WRITE 2; "
                      "ENDIF "
                      "END\n",
              emptyIn, "> 2\n", "shouldCorrectPerformIfCondition_ElseBiggerOrEq"},

        Param{"VAR a b BEGIN a := 2; b := 2; "
                      "IF a <= b THEN "
                      "WRITE 1; "
                      "ELSE "
                      "WRITE 2; "
                      "ENDIF "
                      "END\n",
              emptyIn, "> 1\n", "shouldCorrectPerformIfCondition_ElseLowerOrEq"},
        Param{"VAR a b BEGIN a := 3; b := 2; "
                      "IF a <= b THEN "
                      "WRITE 1; "
                      "ELSE "
                      "WRITE 2; "
                      "ENDIF "
                      "END\n",
              emptyIn, "> 2\n", "shouldCorrectPerformIfCondition_ElseLowerOrEq"},
};


class IfElseTest : public CompilerParamTest {
public:
    IfElseTest()
    {
        Logger::enableDebug = false;
    }
};

TEST_P(IfElseTest, shouldCorrectCompile)
{
    run_test();
}

INSTANTIATE_TEST_CASE_P(, IfElseTest, ::testing::ValuesIn(params));

}  // namespace
