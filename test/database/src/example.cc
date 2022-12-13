#include <gtest/gtest.h>

#include <database/example.hh>

namespace
{

TEST(Database, Example)
{
    EXPECT_EQ(db::return_seven(), 7);
}

}
