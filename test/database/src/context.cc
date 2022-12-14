#include <gtest/gtest.h>

#include <cstring>

#include "database/context.hh"

namespace
{

TEST(Database, Context)
{
    // DATABASE_DIR is defined in CMakeLists.txt
    // and points to the directory with databases
    // needed for unit tests
    db::context ctx{DATABASE_DIR "/context.sqlite"};

    ctx.raw_query(
        "select * from test_context where two=10;",
        []([[maybe_unused]] void* user_data, int argc, char** argv, char** col_name) -> int {
            EXPECT_EQ(std::strcmp(argv[0], "pierogi ruskie"), 0);
            return 0;
        }
    );

    ctx.raw_query(
        "select * from test_context where two=7;",
        []([[maybe_unused]] void* user_data, int argc, char** argv, char** col_name) -> int {
            EXPECT_EQ(std::strcmp(argv[0], "pierogi z miesem"), 0);
            return 0;
        }
    );
}

}  // namespace
