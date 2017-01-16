#include "CompilerTest.hpp"

namespace {
const std::string emptyIn = "";
const std::vector<Param> params = {
    Param{"VAR a BEGIN a := 123; WRITE a; END\n",
          emptyIn,
          "> 123\n",
          "shouldAssignNumberToVar"},
    Param{"VAR a b BEGIN a := 123; b := a; a := 456; WRITE a; WRITE b; END\n",
          emptyIn,
          "> 456\n" "> 123\n",
          "shouldAssignVarToVar"},
    Param{"VAR a b c BEGIN a := 123; c:= 789; b := a; a := 456; WRITE a; WRITE b; END\n",
          emptyIn,
          "> 456\n" "> 123\n",
          "shouldAssignVarToVar"},

    //===========================SUM=====================================

    Param{"VAR a BEGIN a := 12 + 34; WRITE a; END\n",
          emptyIn,
          "> 46\n",
          "shouldAssignSumOfNumbersToVar"},
    Param{"VAR a b BEGIN b := 12; a := b + 34; WRITE a; END\n",
          emptyIn,
          "> 46\n",
          "shouldAssignSumOfVarAndNumberToVar"},
    Param{"VAR a b BEGIN b := 34; a := 12 + b; WRITE a; END\n",
          emptyIn,
          "> 46\n",
          "shouldAssignSumOfNumberAndVarToVar"},
    Param{"VAR a b c BEGIN b := 12; c := 34; a := b + c; WRITE a; END\n",
          emptyIn,
          "> 46\n",
          "shouldAssignSumOfVarsToVar"},

    //===========================SUM=TABS[number]=========================

    Param{"VAR a[10] BEGIN a[5] := 12 + 34; WRITE a[5]; END\n",
          emptyIn,
          "> 46\n",
          "shouldAssignSumOfNumbersToVar_Tabs"},
    Param{"VAR a[10] b[10] BEGIN b[5] := 12; a[5] := b[5] + 34; WRITE a[5]; END\n",
          emptyIn,
          "> 46\n",
          "shouldAssignSumOfVarAndNumberToVar_Tabs"},
    Param{"VAR a[10] b[10] BEGIN b[5] := 34; a[5] := 12 + b[5]; WRITE a[5]; END\n",
          emptyIn,
          "> 46\n",
          "shouldAssignSumOfNumberAndVarToVar_Tabs"},
    Param{"VAR a[10] b[10] c[10] BEGIN b[5] := 12; c[5] := 34; a[5] := b[5] + c[5]; WRITE a[5]; END\n",
          emptyIn,
          "> 46\n",
          "shouldAssignSumOfVarsToVar_Tabs"},

    //===========================SUM=TABS[variable]=========================

    Param{"VAR a[10] d BEGIN d := 6; a[d] := 12 + 34; WRITE a[d]; END\n",
          emptyIn,
          "> 46\n",
          "shouldAssignSumOfNumbersToVar_Tabs[variable]"},
    Param{"VAR a[10] b[10] d BEGIN d := 6; b[d] := 12; a[d] := b[d] + 34; WRITE a[d]; END\n",
          emptyIn,
          "> 46\n",
          "shouldAssignSumOfVarAndNumberToVar_Tabs[variable]"},
    Param{"VAR a[10] b[10] d BEGIN d := 6; b[d] := 34; a[d] := 12 + b[d]; WRITE a[d]; END\n",
          emptyIn,
          "> 46\n",
          "shouldAssignSumOfNumberAndVarToVar_Tabs[variable]"},
    Param{"VAR a[10] b[10] c[10] d BEGIN d := 6; b[d] := 12; c[d] := 34; a[d] := b[d] + c[d]; WRITE a[d]; END\n",
          emptyIn,
          "> 46\n",
          "shouldAssignSumOfVarsToVar_Tabs[variable]"},

    //===========================SUB=====================================

    Param{"VAR a BEGIN a := 43 - 21; WRITE a; END\n",
          emptyIn,
          "> 22\n",
          "shouldAssignSubOfNumbersToVar"},
    Param{"VAR a b BEGIN b := 21; a := 43 - b; WRITE a; END\n",
          emptyIn,
          "> 22\n",
          "shouldAssignSubOfNumberAndVarToVar"},
    Param{"VAR a b BEGIN b := 43; a := b - 21; WRITE a; END\n",
          emptyIn,
          "> 22\n",
          "shouldAssignSubOfVarAndNumberToVar"},
    Param{"VAR a b c BEGIN b := 43; c := 21; a := b - c; WRITE a; END\n",
          emptyIn,
          "> 22\n",
          "shouldAssignSubOfVarsToVar"},
    Param{"VAR a b c BEGIN a := 5; b := 5; c := b - a; WRITE c; END\n",
          emptyIn,
          "> 0\n",
          "shouldAssignSubOfVarsToVar2"},

    //===========================SUB=TABS[number]=========================

    Param{"VAR a[10] BEGIN a[5] := 43 - 21; WRITE a[5]; END\n",
          emptyIn,
          "> 22\n",
          "shouldAssignSubOfNumbersToVar_Tabs"},
    Param{"VAR a[10] b[10] BEGIN b[5] := 43; a[5] := b[5] - 21; WRITE a[5]; END\n",
          emptyIn,
          "> 22\n",
          "shouldAssignSubOfVarAndNumberToVar_Tabs"},
    Param{"VAR a[10] b[10] BEGIN b[5] := 21; a[5] := 43 - b[5]; WRITE a[5]; END\n",
          emptyIn,
          "> 22\n",
          "shouldAssignSubOfNumberAndVarToVar_Tabs"},
    Param{"VAR a[10] b[10] c[10] BEGIN b[5] := 43; c[5] := 21; a[5] := b[5] - c[5]; WRITE a[5]; END\n",
          emptyIn,
          "> 22\n",
          "shouldAssignSubOfVarsToVar_Tabs"},

    //===========================SUB=TABS[variable]=========================

    Param{"VAR a[10] d BEGIN d := 6; a[d] := 43 - 21; WRITE a[d]; END\n",
          emptyIn,
          "> 22\n",
          "shouldAssignSubOfNumbersToVar_Tabs[variable]"},
    Param{"VAR a[10] b[10] d BEGIN d := 6; b[d] := 43; a[d] := b[d] - 21; WRITE a[d]; END\n",
          emptyIn,
          "> 22\n",
          "shouldAssignSubOfVarAndNumberToVar_Tabs[variable]"},
    Param{"VAR a[10] b[10] d BEGIN d := 6; b[d] := 21; a[d] := 43 - b[d]; WRITE a[d]; END\n",
          emptyIn,
          "> 22\n",
          "shouldAssignSubOfNumberAndVarToVar_Tabs[variable]"},
    Param{"VAR a[10] b[10] c[10] d BEGIN d := 6; b[d] := 43; c[d] := 21; a[d] := b[d] - c[d]; WRITE a[d]; END\n",
          emptyIn,
          "> 22\n",
          "shouldAssignSubOfVarsToVar_Tabs[variable]"},

    //======================================================================
    //============================MUL=======================================

//TODO mul z tablicami
    Param{"VAR a BEGIN a := 43 * 21; WRITE a; END\n",
          emptyIn,
          "> 903\n",
          "shouldAssignMulOfNumbersToVar"},
    Param{"VAR a b BEGIN b := 21; a := 43 * b; WRITE a; END\n",
          emptyIn,
          "> 903\n",
          "shouldAssignMulOfNumberAndVarToVar"},
    Param{"VAR a b BEGIN b := 43; a := b * 21; WRITE a; END\n",
          emptyIn,
          "> 903\n",
          "shouldAssignMulOfVarAndNumberToVar"},
    Param{"VAR a b c BEGIN b := 43; c := 21; a := b * c; WRITE a; END\n",
          emptyIn,
          "> 903\n",
          "shouldAssignMulOfVarsToVar"},

    //======================================================================
    //============================DIV=======================================

//TODO div z tablicami
    Param{"VAR a BEGIN a := 89 / 8; WRITE a; END\n",
          emptyIn,
          "> 11\n",
          "shouldAssignDivOfNumbersToVar"},
    Param{"VAR a b BEGIN b := 8; a := 89 / b; WRITE a; END\n",
          emptyIn,
          "> 11\n",
          "shouldAssignDivOfNumberAndVarToVar"},
    Param{"VAR a b BEGIN b := 89; a := b / 8; WRITE a; END\n",
          emptyIn,
          "> 11\n",
          "shouldAssignDivOfVarAndNumberToVar"},
    Param{"VAR a b c BEGIN b := 89; c := 8; a := b / c; WRITE a; END\n",
          emptyIn,
          "> 11\n",
          "shouldAssignDivOfVarsToVar"},


        //======================================================================
        //===========================MODULO=======================================

//TODO mod z tablicami
    Param{"VAR a BEGIN a := 89 % 8; WRITE a; END\n",
          emptyIn,
          "> 1\n",
          "shouldAssignModOfNumbersToVar"},
    Param{"VAR a b BEGIN b := 8; a := 89 % b; WRITE a; END\n",
          emptyIn,
          "> 1\n",
          "shouldAssignModOfNumberAndVarToVar"},
    Param{"VAR a b BEGIN b := 89; a := b % 8; WRITE a; END\n",
          emptyIn,
          "> 1\n",
          "shouldAssignModOfVarAndNumberToVar"},
    Param{"VAR a b c BEGIN b := 89; c := 8; a := b % c; WRITE a; END\n",
          emptyIn,
          "> 1\n",
          "shouldAssignModOfVarsToVar"},

};
} // namespace

class AssignTest : public CompilerParamTest {};

TEST_P(AssignTest, shouldCorrectAssign)
{
    runTest(GetParam());;
}

INSTANTIATE_TEST_CASE_P(, AssignTest, ::testing::ValuesIn(params));
