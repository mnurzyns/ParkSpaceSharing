#include <gtest/gtest.h>

#include <cstring>

#include "database/context.hh"
#include "database/exceptions.hh"

namespace
{

static
constexpr
int
OPEN_FLAGS {SQLITE_OPEN_READONLY};

TEST(Database, Context)
{
    // DATABASE_DIR is defined in CMakeLists.txt
    // and points to the directory with databases
    // needed for unit tests
    db::context ctx{DATABASE_DIR "/context.sqlite", OPEN_FLAGS};

    ctx.raw_query(
        "select * from test_context where two=10;",
        []([[maybe_unused]] void* user_data, [[maybe_unused]] int col_count, char** col_text, [[maybe_unused]] char** col_name) -> int {
            EXPECT_EQ(std::strcmp(col_text[0], "pierogi ruskie"), 0);
            return 0;
        }
    );

    ctx.raw_query(
        "select * from test_context where two=7;",
        []([[maybe_unused]] void* user_data, [[maybe_unused]] int col_count, char** col_text, [[maybe_unused]] char** col_name) -> int {
            EXPECT_EQ(std::strcmp(col_text[0], "pierogi z miesem"), 0);
            return 0;
        }
    );
}

TEST(Database, Query)
{
    db::context ctx{DATABASE_DIR "/context.sqlite", OPEN_FLAGS};

    auto query = ctx.prepare_statement("SELECT * FROM test_context;");
    auto json = query.exec_json();

    EXPECT_EQ(json["one"][0].get<::std::string>(), "pierogi ruskie");
    EXPECT_EQ(json["two"][1].get<int>(), 7);
}

TEST(Database, QueryBindInt)
{
    db::context ctx{DATABASE_DIR "/context.sqlite", OPEN_FLAGS};

    auto query = ctx.prepare_statement("SELECT * FROM test_context WHERE two=?;");
    query.bind(1, 7);

    auto json = query.exec_json();
    EXPECT_EQ(json["one"][0].get<std::string>(), "pierogi z miesem");
}

TEST(Database, QueryBindText)
{
    db::context ctx{DATABASE_DIR "/context.sqlite", OPEN_FLAGS};

    auto query = ctx.prepare_statement("SELECT * FROM test_context WHERE one LIKE ?;");
    query.bind(1, "pierogi z miesem");

    auto json = query.exec_json();
    EXPECT_EQ(json["two"][0].get<int>(), 7);
}

TEST(Database, QueryBindMisuse)
{
    db::context ctx{DATABASE_DIR "/context.sqlite", OPEN_FLAGS};

    auto query = ctx.prepare_statement("SELECT * FROM ?;");
    query.bind(1, "test_context");
    EXPECT_THROW(query.exec(), db::database_misuse);
}

TEST(Database, QueryReset)
{
    db::context ctx{DATABASE_DIR "/context.sqlite", OPEN_FLAGS};

    auto query = ctx.prepare_statement("SELECT * FROM test_context WHERE one LIKE ?;");
    query.bind(1, "pierogi z miesem");

    auto json = query.exec_json();
    EXPECT_EQ(json["two"][0].get<int>(), 7);

    query.reset();
    query.bind(1, "pierogi ruskie");

    json = query.exec_json();
    EXPECT_EQ(json["two"][0].get<int>(), 10);
}

}  // namespace
