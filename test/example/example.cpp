#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "example/Example.hpp"

namespace {

using namespace ::testing;
    
TEST(HelloWorld, Pass)
{
    example::Example ex;
    EXPECT_THAT(ex.getName(), StrEq("Example"));
}

TEST(HelloWorld, Fail)
{
    example::Example ex;
    EXPECT_THAT(ex.getName(), StrEq("NotExample"));
}

}