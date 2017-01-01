#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Driver.hpp"
#include "Interpreter.hpp"
#include "debug.hpp"

namespace {
typedef unsigned value;

class DriverTest : public ::testing::Test {
public:
    DriverTest()
        : driver(generatedCode)
    {
        Logger::enableDebug = false;
    }

    void checkIfCorrectGenerateCode(value& val)
    {
        std::stringstream in;
        std::stringstream out;
        std::stringstream info;
        std::stringstream expected;

        expected << "> " << val << "\n";

        driver.write(jftt::VariableBuilder().withName(std::to_string(val)).isValue().build());
        generatedCode << "HALT\n";
        interpreter.run(generatedCode, in, out, info);


        ASSERT_STREQ(expected.str().c_str(), out.str().c_str())
                << "Param = " << val << "\n"
                << "Generated code: \n"
                << generatedCode.str().c_str();

        generatedCode.str("");
        generatedCode.clear();
    }

    std::stringstream generatedCode;

    jftt::Driver driver;
    Interpreter interpreter;

};

TEST_F(DriverTest, shouldCorrectGenerateCodeToShowNumber_smallNumber) {
    for(value val = 0 ; val < 1000 ; ++val)
    {
        checkIfCorrectGenerateCode(val);
    }
}

}  // namespace
