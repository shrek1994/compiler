#include <gtest/gtest.h>
int main(int numOfArg, char *args[])
{
    ::testing::InitGoogleTest(&numOfArg, args);
    return RUN_ALL_TESTS();
}