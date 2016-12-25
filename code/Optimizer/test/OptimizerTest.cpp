#include <Optimizer.hpp>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <memory>

namespace optimizer {


class OptimizerTest : public ::testing::Test
{
public:
    OptimizerTest() = default;
    virtual ~OptimizerTest() = default;

    void SetUp() override
    {
        in.str("");
        out.str("");
        info.str("");
        error.str("");
    }

    std::shared_ptr<Optimizer> optimizer;

    std::stringstream in, out, info, error;
};


TEST_F(OptimizerTest, shouldCheckEmptyCodeCorrectly)
{
    in << "VAR BEGIN SKIP; END\n";
    auto out = optimizer->run(in);
    ASSERT_STREQ(in.str().c_str(), out.str().c_str());
}

}