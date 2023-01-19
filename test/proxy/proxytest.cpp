#include <gtest/gtest.h>
#include "proxy/proxy.hpp"

namespace {

using namespace ::testing;

TEST(testfunction_pass, Pass)
{
    proxy::Proxy t;
    EXPECT_EQ(t.testfunction(1), 1);
}

TEST(testfunction_fail, Fail)
{
    proxy::Proxy t;
    EXPECT_NE(t.testfunction(1), 0);
}

}
