#include <Checker.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>

namespace checker {


class CheckerTest : public ::testing::Test
{
public:
    CheckerTest() = default;
    virtual ~CheckerTest() = default;

    void SetUp() override
    {
        in.str("");
        out.str("");
        info.str("");
        error.str("");
    }

    std::shared_ptr<Checker> checker;

    std::stringstream in, out, info, error;
};


    TEST_F(CheckerTest, shouldCheckEmptyCodeCorrectly)
    {
        in << "VAR BEGIN SKIP; END\n";
        auto out = checker->run(in);
        ASSERT_STREQ(in.str().c_str(), out.str().c_str());
    }

}