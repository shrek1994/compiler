#include "CompilerTest.hpp"
#include "debug.hpp"

namespace {
const std::string emptyIn = "";

const std::vector<Param> params = {
        Param{"VAR a b BEGIN "
              "a := 1;"
              "b := 5;"
              "WHILE a < b DO "
              "WRITE a; "
              "a := a + 1; "
              "ENDWHILE"
              " END\n",
              emptyIn,
              "> 1\n> 2\n> 3\n> 4\n",
              "shouldCorrectPerformWhile_lower"},
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
