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
              emptyIn, "> 1", "shouldCompileEmptyCodeCorrectly"},
};


class IfElseTest : public CompilerParamTest {
public:
    IfElseTest()
    {
        Logger::enableDebug = true;
    }
};

TEST_P(IfElseTest, DISABLED_shouldCorrectCompile)
{
    run_test();
}

INSTANTIATE_TEST_CASE_P(, IfElseTest, ::testing::ValuesIn(params));

}  // namespace
