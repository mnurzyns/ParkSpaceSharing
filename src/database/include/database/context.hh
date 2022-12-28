#pragma once

#include <filesystem>

#include <sqlite3.h>

namespace db
{

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
    context(std::filesystem::path const& path);

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

private:
    ::sqlite3* handle_;

};

} // namespace db
