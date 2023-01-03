#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Proxy/proxy.hpp"

namespace {

using namespace ::testing;
    
TEST(testfunction_pass, Pass)
{
    proxy::proxy t;
    EXPECT_EQ(t.testfunction(1), 1);
}

TEST(testfunction_fail, Fail)
{
    proxy::proxy t;
    EXPECT_EQ(t.testfunction(1), 0);
}

}