#pragma once

#include "database/detail/prepare_insert.hh"
#include "database/insert_descriptor.hh"
#include "database/statement.hh"

#include <filesystem>
#include <utility>

#include <sqlite3.h>

#include <nlohmann/json.hpp>

namespace db
{

namespace detail
{

template<class Type>
concept insert_array =
requires(Type&& type)
{
    { type.begin() };
    { type.end()   };
    { type.size()  };
    { typename std::decay_t<Type>::value_type{} };
};

} // namespace detail

/**
 * @brief Callback for sqlite3 queries
 *
 * @returns 0 on success, non-zero stops the execution of any subsequent SQL statements.
 */
using query_cb = int(*)(void* user_data, int col_count, char** col_text, char** col_names);

/**
 * @brief Context for database operations.
 * Contains the handle to sqlite3 database.
 *
 * @note This class can not be copied.
 */
class context
{

public:
    /**
     * @brief Opens the database and initializes the handle.
     * The database is opened with write permissions.
     *
     * @throws db::database_open_error when the database could not be opened and prints the error to <tt>stderr</tt>.
     * @param path Valid path to the sqlite3 database.
     */
    [[nodiscard]]
    explicit
    context(std::filesystem::path const& path, int flags = DEFAULT_OPEN_FLAGS);

    ~context();

    context(context const&) = delete;

    [[nodiscard]]
    context(context&&) noexcept;

    context&
    operator=(context const&) = delete;

    [[nodiscard]]
    context&
    operator=(context&&) noexcept;

    /**
     * @brief Prepares a statement.
     * A statement is used to execute databasee queries.
     *
     * @param str Query string
     */
    [[nodiscard]]
    db::statement
    prepare_statement(char const* str) noexcept;

    /**
     * @brief Prepare an INSERT statement.
     *
     * @param value Structure described with <i>db::insert_descriptor</i>
     * @see db::insert_descriptor
     */
    template<class Type>
    [[nodiscard]]
    db::statement
    prepare_insert(Type&& value)
    {
        auto str = detail::prepare_insert_string<Type>(1);
        auto statement = this->prepare_statement(str.c_str());

        int arg_i = 1;
        detail::prepare_insert_bind(statement, arg_i, std::forward<Type>(value));

        return statement;
    }

    /**
     * @brief Prepare an INSERT statement.
     *
     * @param value Container with structures described with <i>db::insert_descriptor</i>
     * @see db::insert_descriptor
     */
    template<class Type>
    requires(detail::insert_array<Type>)
    [[nodiscard]]
    db::statement
    prepare_insert(Type&& container)
    {
        using value_t = typename std::decay_t<Type>::value_type;

        auto insert_string = detail::prepare_insert_string<value_t>(container.size());
        auto statement = this->prepare_statement(insert_string.c_str());

        int arg_i = 1;
        for(std::size_t i=0;i<container.size();++i) {
            detail::prepare_insert_bind(statement, arg_i, container[i]);
        }

        return statement;
    }

    /**
     * @brief Executes a raw query.
     *
     * @attention Failed query is considered a bug and should not happen.
     * @throws db::database_query_error when query fails and prints the error to <tt>stderr</tt>.
     */
    void
    raw_query(
        char const* query,
        query_cb callback,
        void* user_data = nullptr
    );

    static
    constexpr
    int
    DEFAULT_OPEN_FLAGS {SQLITE_OPEN_READWRITE};

private:
    ::sqlite3* handle_;

};

} // namespace db
