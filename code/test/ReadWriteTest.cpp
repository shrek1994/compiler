#include "CompilerTest.hpp"

namespace {

const std::vector<Param> params = {
    Param{"VAR BEGIN WRITE 123; END\n",
          "",
          "> 123\n",
          "shouldCompileEmptyCodeCorrectly"},
    Param{"VAR liczba BEGIN READ liczba; WRITE liczba; END\n",
          "123\n",
          "? > 123\n",
          "shouldReadVarAndShowIt"},
    Param{"VAR b[100] BEGIN READ b[99]; READ b[98]; WRITE b[99]; END\n",
          "123\n" "456\n",
          "? ? > 123\n",
          "shouldCorrectlyShowNumberAtLastElemInTab"},
    Param{"VAR a b[100] BEGIN READ a; READ b[a]; WRITE a; WRITE b[5]; WRITE b[a]; END\n",
          "5\n" "123\n",
          "? ? > 5\n" "> 123\n" "> 123\n",
          "shouldCorrectlyShowNumberWhenIndexIsVariable"},
    Param{"VAR a b[100] BEGIN READ a; READ b[a]; READ b[69]; WRITE b[a]; WRITE b[5]; WRITE b[69]; END\n",
          "5\n" "123\n" "456\n",
          "? ? ? > 123\n" "> 123\n" "> 456\n",
          "shouldCorrectlyShowNumberWhenIndexIsVariable"}
};

} // namespace

class ReadWriteTest : public CompilerParamTest
{
public:
    ReadWriteTest()
    {
    }
};

TEST_P(ReadWriteTest, shouldCorrectCompile)
{
    runTest(GetParam());
}

INSTANTIATE_TEST_CASE_P(, ReadWriteTest, ::testing::ValuesIn(params));
