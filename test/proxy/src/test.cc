#include <gtest/gtest.h>

#include "proxy/test.hh"

namespace
{

TEST(Proxy, test)
{
    EXPECT_EQ(proxy::test(), 10);
}

}
