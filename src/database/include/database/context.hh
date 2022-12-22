#pragma once

#include <filesystem>
#include <functional>
#include <string_view>

#include <sqlite3.h>

namespace db
{

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
     * @throws std::runtime_error when the database could not be opened and prints the error to <tt>stderr</tt>.
     * @param path Valid path to the sqlite3 database.
     */
    [[nodiscard]]
    explicit
    context(std::filesystem::path const& path);

    ~context();

    context(context const&) = delete;
    context(context&&) = default;

    context&
    operator=(context const&) = delete;

    [[nodiscard]]
    context&
    operator=(context&&) = default;

    /**
     * @brief Executes a raw query.
     *
     * @attention Failed query is considered a bug and should not happen.
     * @throws std::runtime_error when query fails and prints the error to <tt>stderr</tt>.
     */
    void
    raw_query(
        char const* query,
        int(*callback)(void*,int,char**,char**),
        void* user_data = nullptr
    );

private:
    ::sqlite3* handle_;

};

} // namespace db
