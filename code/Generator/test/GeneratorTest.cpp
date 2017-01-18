#include <gtest/gtest.h>
#include <debug.hpp>
#include <Generator.hpp>
#include <memory>
#include <Interpreter.hpp>
#include <Variable.hpp>

namespace {

class GeneratorTest : public ::testing::Test {
public:
    GeneratorTest() = default;

    virtual ~GeneratorTest() = default;

    void SetUp() override {
        Logger::enableDebug = false;
        code.str("");
        generatedCode.str("");
        in.str("");
        out.str("");
        info.str("");
        error.str("");
        expected.str("");
    }

    std::stringstream code, generatedCode, in, out, info, error, expected;
    std::shared_ptr<Generator> gen = std::make_shared<Generator>(generatedCode);
    std::shared_ptr<Interpreter> inter = std::make_shared<Interpreter>();
};

TEST_F(GeneratorTest, shouldCorrectCompileIf)
{
    expected << "> 1\n> 100\n";
    code << "BEGIN\n"
            "a := 1;\n"
            "b := 2;\n"
            "$reg1 := b - a;\n"
            "JZERO 1 %ELSE0%;\n"
            "WRITE a;\n"
            "JUMP %ENDIF0%;\n"
            "%ELSE0%: WRITE b;\n"
            "%ENDIF0%: WRITE 100;\n"
        "END\n";

    gen->run(code, { DEFAULT_VAR, "a", "b"});
    inter->run(generatedCode, in, out, info);

    EXPECT_STREQ(expected.str().c_str(), out.str().c_str())
        << "code:\n"
        << code.str()
        << "\n"
        << "generated code:\n"
        << generatedCode.str();
}

TEST_F(GeneratorTest, shouldCorrectCompileWhile) {
    expected << "> 1\n> 2\n> 3\n> 4\n";
    code << "BEGIN\n"
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
            "END\n";

    gen->run(code, { DEFAULT_VAR, "a", "b"});
    inter->run(generatedCode, in, out, info);

    EXPECT_STREQ(expected.str().c_str(), out.str().c_str())
                        << "code:\n"
                        << code.str()
                        << "\n"
                        << "generated code:\n"
                        << generatedCode.str();
}

TEST_F(GeneratorTest, shouldCorrectMul)
{
    expected << "> 35\n";
    code <<
         "BEGIN\n"
             "a := 5;\n"
             "b := 7;\n"
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

    gen->run(code, {DEFAULT_VAR, "a", "b", "c"});
    inter->run(generatedCode, in, out, info);

    EXPECT_STREQ(expected.str().c_str(), out.str().c_str())
                        << "code:\n"
                        << code.str()
                        << "\n"
                        << "generated code:\n"
                        << generatedCode.str();
}


TEST_F(GeneratorTest, shouldCorrectDiv)
{
    expected << "> 12\n";
    code <<
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

    gen->run(code, { DEFAULT_VAR, "a", "b", "c"});
    inter->run(generatedCode, in, out, info);

    EXPECT_STREQ(expected.str().c_str(), out.str().c_str())
                        << "code:\n"
                        << code.str()
                        << "\n"
                        << "generated code:\n"
                        << generatedCode.str();
}

TEST_F(GeneratorTest, shouldCorrectModulo)
{
    expected << "> 2\n";
    code <<
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

    gen->run(code, { DEFAULT_VAR, "a", "b", "c"});
    inter->run(generatedCode, in, out, info);

    EXPECT_STREQ(expected.str().c_str(), out.str().c_str())
                        << "code:\n"
                        << code.str()
                        << "\n"
                        << "generated code:\n"
                        << generatedCode.str();
}


}