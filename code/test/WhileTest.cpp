#include "CompilerTest.hpp"
#include "debug.hpp"

namespace {
const std::string emptyIn = "";

const std::vector<Param> params = {
        Param{"VAR a b BEGIN a := 1; b := 5; "
              "WHILE a < b DO "
              "WRITE a; "
              "a := a + 1; "
              "ENDWHILE"
              " END\n",
              emptyIn,
              "> 1\n> 2\n> 3\n> 4\n",
              "shouldCorrectPerformWhile_lower"},
        Param{"VAR a b BEGIN a := 1; b := 5; "
              "WHILE a <= b DO "
              "WRITE a; "
              "a := a + 1; "
              "ENDWHILE"
              " END\n",
              emptyIn,
              "> 1\n> 2\n> 3\n> 4\n> 5\n",
              "shouldCorrectPerformWhile_lowerOrEq"},
        Param{"VAR a b BEGIN a := 5; b := 1; "
              "WHILE a > b DO "
              "WRITE a; "
              "a := a - 1; "
              "ENDWHILE"
              " END\n",
              emptyIn,
              "> 5\n> 4\n> 3\n> 2\n",
              "shouldCorrectPerformWhile_bigger"},
        Param{"VAR a b BEGIN a := 5; b := 1; "
              "WHILE a >= b DO "
              "WRITE a; "
              "a := a - 1; "
              "ENDWHILE"
              " END\n",
              emptyIn,
              "> 5\n> 4\n> 3\n> 2\n> 1\n",
              "shouldCorrectPerformWhile_biggerOrEq"},
        Param{"VAR a b BEGIN a := 5; b := 1; "
              "WHILE a <> b DO "
              "WRITE a; "
              "a := a - 1; "
              "ENDWHILE"
              " END\n",
              emptyIn,
              "> 5\n> 4\n> 3\n> 2\n",
              "shouldCorrectPerformWhile_NotEq"},
        Param{"VAR a b BEGIN a := 5; b := 5; "
              "WHILE a = b DO "
              "WRITE a; "
              "a := a - 1; "
              "ENDWHILE"
              " END\n",
              emptyIn,
              "> 5\n",
              "shouldCorrectPerformWhile_NotEq"},
};

} // namespace

class WhileTest : public CompilerParamTest
{
public:
    WhileTest()
    {
        Logger::enableDebug = false;
    }
};

TEST_P(WhileTest, shouldCorrectCompile)
{
    run_test();
}

INSTANTIATE_TEST_CASE_P(, WhileTest, ::testing::ValuesIn(params));
