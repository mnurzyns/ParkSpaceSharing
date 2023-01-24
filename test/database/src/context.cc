#include <gtest/gtest.h>

#include <cstring>

#include "database/context.hh"
#include "database/exceptions.hh"

struct user_test
{
    int id;
    std::string name;
    std::string password;
};

template<>
struct db::insert_descriptor<user_test>
{
    static
    constexpr
    char const*
    TABLE{"user"};

    static
    constexpr
    std::tuple
    COLUMNS
    {
        std::pair{&user_test::name, "name"},
        std::pair{&user_test::password, "password"},
    };
};

namespace
{

constexpr
int OPEN_FLAGS {SQLITE_OPEN_READONLY};

TEST(Database, Context)
{
    // DATABASE_DIR is defined in CMakeLists.txt
    // and points to the directory with databases
    // needed for unit tests
    db::context ctx{DATABASE_DIR "/context.sqlite", OPEN_FLAGS};

    ctx.raw_query(
        "select * from test_context where two=10;",
        //NOLINTNEXTLINE
        []([[maybe_unused]] void* user_data, [[maybe_unused]] int col_count, char** col_text, [[maybe_unused]] char** col_name) -> int {
            EXPECT_EQ(std::strcmp(col_text[0], "pierogi ruskie"), 0); //NOLINT
            return 0;
        }
    );

    ctx.raw_query(
        "select * from test_context where two=7;",
        //NOLINTNEXTLINE
        []([[maybe_unused]] void* user_data, [[maybe_unused]] int col_count, char** col_text, [[maybe_unused]] char** col_name) -> int {
            EXPECT_EQ(std::strcmp(col_text[0], "pierogi z miesem"), 0); //NOLINT
            return 0;
        }
    );
}

TEST(Database, Statement)
{
    db::context ctx{DATABASE_DIR "/context.sqlite", OPEN_FLAGS};

    auto json = ctx
        .prepare_statement("SELECT * FROM test_context;")
        .exec_json();

    EXPECT_EQ(json["one"][0].get<::std::string>(), "pierogi ruskie");
    EXPECT_EQ(json["two"][1].get<int>(), 7);
}

TEST(Database, StatementBindInt)
{
    db::context ctx{DATABASE_DIR "/context.sqlite", OPEN_FLAGS};

    auto statement = ctx.prepare_statement("SELECT * FROM test_context WHERE two=?;");
    statement.bind(1, 7); //NOLINT

    auto json = statement.exec_json();

    EXPECT_EQ(json["one"][0].get<std::string>(), "pierogi z miesem");
}

TEST(Database, StatementBindText)
{
    db::context ctx{DATABASE_DIR "/context.sqlite", OPEN_FLAGS};

    auto statement = ctx.prepare_statement("SELECT * FROM test_context WHERE one LIKE ?;");
    statement.bind(1, "pierogi z miesem");

    auto json = statement.exec_json();
    EXPECT_EQ(json["two"][0].get<int>(), 7);
}

TEST(Database, StatementBindCStr)
{
    db::context ctx{DATABASE_DIR "/context.sqlite", OPEN_FLAGS};

    std::string str{"pierogi z miesem"};
    auto statement = ctx.prepare_statement("SELECT * FROM test_context WHERE one LIKE ?;");
    statement.bind(1, str);

    auto json = statement.exec_json();
    EXPECT_EQ(json["two"][0].get<int>(), 7);
}

TEST(Database, StatementBindMisuse)
{
    db::context ctx{DATABASE_DIR "/context.sqlite", OPEN_FLAGS};

    auto statement = ctx.prepare_statement("SELECT * FROM ?;");
    statement.bind(1, "test_context");

    EXPECT_THROW(statement.exec(), db::database_misuse);
}

TEST(Database, StatementReset)
{
    db::context ctx{DATABASE_DIR "/context.sqlite", OPEN_FLAGS};

    auto statement = ctx.prepare_statement("SELECT * FROM test_context WHERE one LIKE ?;");
    statement.bind(1, "pierogi z miesem");

    auto json = statement.exec_json();
    EXPECT_EQ(json["two"][0].get<int>(), 7);

    statement.reset();
    statement.bind(1, "pierogi ruskie");

    json = statement.exec_json();
    EXPECT_EQ(json["two"][0].get<int>(), 10);
}

TEST(Database, StatementInsertOne)
{
    db::context ctx{":memory:"};

    ctx.prepare_statement(
        "CREATE TABLE `user` (`id` integer primary key, `name` text not null, `password` text not null);"
    ).exec();

    user_test user{0, "abc", "pass"};
    ctx
        .prepare_insert(user)
        .exec();

    auto json = ctx
        .prepare_statement("SELECT * FROM `user`;")
        .exec_json();

    EXPECT_EQ(json["name"][0], "abc");
    EXPECT_EQ(json["password"][0], "pass");
}

TEST(Database, StatementInsertArray)
{
    db::context ctx{":memory:"};

    ctx.prepare_statement(
        "CREATE TABLE `user` (`id` integer primary key, `name` text not null, `password` text not null);"
    ).exec();

    std::array array{
        user_test{0, "abc0", "pass0"},
        user_test{1, "abc1", "pass1"},
    };

    ctx
        .prepare_insert(array)
        .exec();

    auto json = ctx
        .prepare_statement("SELECT * FROM `user`;")
        .exec_json();

    EXPECT_EQ(json["name"][0],     "abc0");
    EXPECT_EQ(json["password"][0], "pass0");
    EXPECT_EQ(json["name"][1],     "abc1");
    EXPECT_EQ(json["password"][1], "pass1");
}

}  // namespace
