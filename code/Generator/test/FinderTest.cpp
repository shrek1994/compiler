#include <gtest/gtest.h>
#include <Finder.hpp>

namespace {

class FinderTest : public ::testing::Test {
public:
    Finder sut;
    std::stringstream in, out, expected;
};

TEST_F(FinderTest, shouldCorrectFindPosition)
{
    std::map<std::string, unsigned long> expectedPosition = {
            {"%LAST0%", 1},
            {"%LAST1%", 4},
            {"%END1%", 5}
    };
    in << "JUMP %LAST1%\n"
          "%LAST0%: JZERO 2 %END1%\n"
          "ZERO 1\n"
          "JUMP %LAST0%\n"
          "%LAST1%: JUMP %END1%;\n"
          "%END1%: HALT\n";
    expected << "JUMP %LAST1%\n"
           " JZERO 2 %END1%\n"
           "ZERO 1\n"
           "JUMP %LAST0%\n"
           " JUMP %END1%;\n"
           " HALT\n";

    auto& postion = sut.run(in, out);
    EXPECT_STREQ(expected.str().c_str(), out.str().c_str());
    EXPECT_EQ(expectedPosition, postion);

}

}