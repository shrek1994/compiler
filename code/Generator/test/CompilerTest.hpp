#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "Compiler.hpp"
#include "Interpreter.hpp"
#include "debug.hpp"
#include <sstream>

#define run_test() \
    language << GetParam().code; \
    in << GetParam().in; \
     \
    compiler.run(language, generatedCode); \
    interpreter.run(generatedCode, in, out, info); \
     \
    EXPECT_STREQ(GetParam().expected.c_str(), out.str().c_str()) \
            << GetParam().testName << "\n" \
            << "\n" \
            << "code:\n" \
            << GetParam().code \
            << "\n" \
            << "generated code:\n" \
            << generatedCode.str()
//            << info.str()


struct Param
{
    std::string code;
    std::string in;
    std::string expected;
    std::string testName;
};
std::ostream& operator << (std::ostream& out, Param param);


class CompilerTest : public ::testing::Test
{
public:
    CompilerTest()
    {
        Logger::enableDebug = false;
    }

    virtual ~CompilerTest() = default;

    void SetUp() override
    {
        language.str("");
        generatedCode.str("");
        in.str("");
        out.str("");
        info.str("");
    }

    Compiler compiler;
    Interpreter interpreter;

    std::stringstream language, generatedCode, in, out, info;
};

class CompilerParamTest : public CompilerTest,
                          public ::testing::WithParamInterface<Param> {};
