#pragma once
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "compiler.hpp"
#include "Interpreter.hpp"
#include "debug.hpp"
#include <sstream>
#include <future>

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
        error.str("");
    }

    void runTest(const Param& param)
    {
        language << param.code;
        in << param.in;
        DEBUG << param.testName << "\n";

        auto executionTest = std::async(std::launch::async, [&](){
            compiler.run(language, generatedCode);
            interpreter.run(generatedCode, in, out, info);
        });

        ASSERT_TRUE(executionTest.wait_for(executionTime) != std::future_status::timeout) << param.testName << "\n";
        EXPECT_STREQ(param.expected.c_str(), out.str().c_str())
                << param.testName << "\n"
                << info.str()
                << error.str()
                << "code:\n"
                << param.code
                << "\n"
                << "generated code:\n"
                << generatedCode.str();
    }
    std::stringstream language, generatedCode, in, out, info, error;

    jftt::compiler compiler{info, error};
    Interpreter interpreter;

    std::chrono::seconds executionTime = std::chrono::seconds(5);
};

class CompilerParamTest : public CompilerTest,
                          public ::testing::WithParamInterface<Param> {};
