#include <gtest/gtest.h>
#include <Replacer.hpp>
#include <map>

namespace {

class ReplacerTest : public ::testing::Test {
public:
    Replacer sut;
    std::stringstream in, out, expected;
};

TEST_F(ReplacerTest, shouldCorrectReplacePosition)
{
    std::map<std::string, unsigned long> position = {
            {"%LAST0%", 2},
            {"%LAST1%", 5},
            {"%END1%", 6}
    };
    in << "JUMP %LAST1%\n"
            " JZERO 2 %END1%\n"
            "ZERO 1\n"
            "JUMP %LAST0%\n"
            " JUMP %END1%;\n"
            " HALT\n";
    expected << "JUMP 5\n"
            " JZERO 2 6\n"
            "ZERO 1\n"
            "JUMP 2\n"
            " JUMP 6;\n"
            " HALT\n";

    sut.run(in, out, position);
    EXPECT_STREQ(expected.str().c_str(), out.str().c_str());
}

}