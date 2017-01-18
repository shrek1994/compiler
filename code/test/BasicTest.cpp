#include "CompilerTest.hpp"

namespace {
const std::string emptyIn = "";
const std::string expectEmptyOut = "";
const std::vector<Param> params = {
    Param{"VAR BEGIN SKIP; END\n",       emptyIn, expectEmptyOut, "shouldCompileEmptyCodeCorrectly"},
    Param{"VAR a BEGIN SKIP; END\n",     emptyIn, expectEmptyOut, "shouldCompileCorrectlyCodeWithOnlyOneDeclaration"},
    Param{"VAR a b c BEGIN SKIP; END\n", emptyIn, expectEmptyOut, "shouldCompileCorrectlyCodeWithManyDeclaration"},
    Param{"VAR a[10] BEGIN SKIP; END\n", emptyIn, expectEmptyOut, "shouldCompileCorrectlyCodeWithOnlyOneTabDeclaration"},
    Param{"VAR a[10] b[5] c[1] BEGIN SKIP; END\n", emptyIn, expectEmptyOut, "shouldCompileCorrectlyCodeWithManyTabDeclaration"},
    Param{"VAR a b c BEGIN \n"
          "b := 1;"
          "a := b ;\n"
          "c := a + b ;\n"
          "c := a - b ;\n"
          "c := a * b ;\n"
          "c := a / b ;\n"
          "c := a % b ;\n"
          "END\n",
          emptyIn, expectEmptyOut, "shouldCompileCorrectlyCodeWithAllExpressions"},
    Param{"VAR a b BEGIN \
           IF a = b THEN SKIP; ELSE SKIP; ENDIF\
           IF a <> b THEN SKIP; ELSE SKIP; ENDIF\
           IF a < b THEN SKIP; ELSE SKIP; ENDIF\
           IF a > b THEN SKIP; ELSE SKIP; ENDIF\
           IF a <= b THEN SKIP; ELSE SKIP; ENDIF\
           IF a >= b THEN SKIP; ELSE SKIP; ENDIF\
           END\n",
          emptyIn, expectEmptyOut, "shouldCompileCorrectlyCodeWithAllConditions"},
    Param{"VAR a b BEGIN\n"
          "a := 2; b := 5"
          "WHILE a < b DO a := a + 1; ENDWHILE\n"
          "END\n",
          emptyIn, expectEmptyOut, "shouldCompileWhileCorrectly"},
    Param{"VAR a b BEGIN\n"
          "FOR i FROM 1 TO 10 DO SKIP; ENDFOR\n"
          "END\n",
          emptyIn, expectEmptyOut, "shouldCompileForToCorrectly"},
    Param{"VAR a b BEGIN\n"
          "FOR i FROM 10 DOWNTO 1 DO SKIP; ENDFOR\n"
          "END\n",
          emptyIn, expectEmptyOut, "shouldCompileForDownToCorrectly"},
};
} // namespace

class BasicTest : public CompilerParamTest {};

TEST_P(BasicTest, shouldCorrectCompile)
{
    runTest(GetParam());
}

INSTANTIATE_TEST_CASE_P(, BasicTest, ::testing::ValuesIn(params));
