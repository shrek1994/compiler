#include "CompilerTest.hpp"

namespace {
const std::string emptyIn = "";

const std::vector<Param> params = {
        Param{"VAR a b BEGIN a := 1; b := 5; \n"
                      "FOR i FROM a TO b DO\n"
                      "WRITE i;\n"
                      "ENDFOR\n"
                      " END\n",
              emptyIn,
              "> 1\n> 2\n> 3\n> 4\n> 5\n",
              "shouldCorrectPerformFor_to"},
        Param{"VAR a b BEGIN a := 1; b := 5; \n"
              "FOR i FROM b DOWNTO a DO\n"
                "WRITE i;\n"
              "ENDFOR\n"
              " END\n",
              emptyIn,
              "> 5\n> 4\n> 3\n> 2\n> 1\n",
              "shouldCorrectPerformFor_downTo"},
};

} // namespace

class ForTest : public CompilerParamTest
{
public:
    ForTest()
    {
        Logger::enableDebug = false;
    }
};

TEST_P(ForTest, shouldCorrectCompile)
{
    runTest(GetParam());
}

INSTANTIATE_TEST_CASE_P(, ForTest, ::testing::ValuesIn(params));
