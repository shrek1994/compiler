#include <gtest/gtest.h>
#include <debug.hpp>
#include <Generator.hpp>
#include <memory>
#include <Interpreter.hpp>

namespace {

class GeneratorTest : public ::testing::Test {
public:
    GeneratorTest() = default;

    virtual ~GeneratorTest() = default;

    void SetUp() override {
        Logger::enableDebug = true;
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

TEST_F(GeneratorTest, DISABLED_shouldCorrectCompileIf)
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

    gen->run(code, {"a", "b"});
    inter->run(generatedCode, in, out, info);

    EXPECT_STREQ(expected.str().c_str(), out.str().c_str())
        << "code:\n"
        << code.str()
        << "\n"
        << "generated code:\n"
        << generatedCode.str();
}

}