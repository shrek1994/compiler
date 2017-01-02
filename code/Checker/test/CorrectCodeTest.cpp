#include <debug.hpp>
#include <Checker.hpp>
#include <gtest/gtest.h>

namespace checker {

namespace {

struct Param
{
    std::string code;
    std::string in;
    std::string expected;
    std::string testName;
};

const std::string emptyIn = "";
const std::string expectEmptyOut = "";
const std::vector<Param> params = {
        Param{"VAR BEGIN\nSKIP;\nEND\n",       emptyIn, expectEmptyOut, "shouldCompileEmptyCodeCorrectly"},
        Param{"VAR a BEGIN\nSKIP;\nEND\n",     emptyIn, expectEmptyOut, "shouldCompileCorrectlyCodeWithOnlyOneDeclaration"},
        Param{"VAR a b c BEGIN\nSKIP;\nEND\n", emptyIn, expectEmptyOut, "shouldCompileCorrectlyCodeWithManyDeclaration"},
        Param{"VAR a[10] BEGIN\nSKIP;\nEND\n", emptyIn, expectEmptyOut, "shouldCompileCorrectlyCodeWithOnlyOneTabDeclaration"},
        Param{"VAR a[10] b[5] c[1] BEGIN\nSKIP;\nEND\n", emptyIn, expectEmptyOut, "shouldCompileCorrectlyCodeWithManyTabDeclaration"},
        Param{"VAR a b BEGIN\n"
                      "a := b;\n"
                      "a := a + b;\n"
                      "a := a - b;\n"
                      "a := a * b;\n"
                      "a := a / b;\n"
                      "a := a % b;\n"
                      "END\n",
              emptyIn, expectEmptyOut, "shouldCompileCorrectlyCodeWithAllExpressions"},
        Param{"VAR a b BEGIN\n"
          "IF a = b THEN\nSKIP;\nELSE\nSKIP;\nENDIF\n"
          "IF a <> b THEN\nSKIP;\nELSE\nSKIP;\nENDIF\n"
          "IF a < b THEN\nSKIP;\nELSE\nSKIP;\nENDIF\n"
           "IF a > b THEN\nSKIP;\nELSE\nSKIP;\nENDIF\n"
           "IF a <= b THEN\nSKIP;\nELSE\nSKIP;\nENDIF\n"
           "IF a >= b THEN\nSKIP;\nELSE\nSKIP;\nENDIF\n"
           "END\n",
              emptyIn, expectEmptyOut, "shouldCompileCorrectlyCodeWithAllConditions"},
        Param{"VAR a b BEGIN\n"
                      "WHILE a < b DO\nSKIP;\nENDWHILE\n"
                      "END\n",
              emptyIn, expectEmptyOut, "shouldCompileWhileCorrectly"},
        Param{"VAR a b BEGIN\n"
                      "FOR i FROM 1 TO 10 DO\nSKIP;\nENDFOR\n"
                      "END\n",
              emptyIn, expectEmptyOut, "shouldCompileForToCorrectly"},
        Param{"VAR a b BEGIN\n"
                      "FOR i FROM 10 DOWNTO 1 DO\nSKIP;\nENDFOR\n"
                      "END\n",
              emptyIn, expectEmptyOut, "shouldCompileForDownToCorrectly"},
};
} // namespace

class CorrectCodeTest : public ::testing::Test {
public:
    CorrectCodeTest() = default;

    virtual ~CorrectCodeTest() = default;

    void SetUp() override {
        Logger::enableDebug = false;
        in.str("");
        out.str("");
        info.str("");
        error.str("");
        expected.str("");
    }

    std::stringstream in, out, info, error, expected;
    std::shared_ptr <Checker> checker = std::make_shared<Checker>(out);
};

class CorrectCodeTestParamTest : public CorrectCodeTest,
                          public ::testing::WithParamInterface<Param> {};

TEST_P(CorrectCodeTestParamTest, shouldCorrectCompile)
{
    in << GetParam().code;

    checker->run(in);

    auto outString = in.str();
    auto position = outString.find("BEGIN");
    EXPECT_STREQ(GetParam().code.substr(position).c_str(), out.str().c_str()) \
            << GetParam().testName << "\n" \
            << "\n" \
            << "code:\n" \
            << GetParam().code \
            << "\n" \
            << "generated code:\n" \
            << out.str();
}

INSTANTIATE_TEST_CASE_P(, CorrectCodeTestParamTest, ::testing::ValuesIn(params));
} //namespace checker