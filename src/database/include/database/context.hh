#pragma once

#include <stdalign.h>
#include <cstring>
#include <filesystem>

#include <sqlite3.h>
#include <utility>

#include "descriptor.hh"

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

    template<class T>
    T
    select_one()
    {
        auto desc = db::descriptor<T>::get();

        std::string query{"SELECT * FROM "};
        query += desc.second;
        query += ";";

        T ret{};

//int(*)(void* user_data, int col_count, char** col_text, char** col_names);
        auto res = ::sqlite3_exec(handle_, query.c_str(),
            [](
                void* user_data,
                int col_count,
                char** col_text,
                char** col_names
            ) -> int{
                auto desc = db::descriptor<T>::get();
                [&]<class... Args>(std::tuple<Args...> tup) {
                    [&]<std::size_t... Index>(std::integer_sequence<std::size_t, Index...>) {
                        (..., [&](auto pair){
                         for(int name_i = 0; name_i < col_count; ++name_i) {
                            if(std::strcmp(col_names[name_i], pair.second) == 0) {
                                if constexpr(std::is_same_v<std::string, std::decay_t<decltype((*static_cast<T*>(user_data)).*pair.first)>>) {
                                    (*static_cast<T*>(user_data)).*pair.first = col_text[name_i];
                                } else {
                                    (*static_cast<T*>(user_data)).*pair.first = std::atoi(col_text[name_i]);
                                }
                            }
                         }
                         }(std::get<Index>(tup)));
                    }(std::make_integer_sequence<std::size_t, sizeof...(Args)>());
                }(desc.first);
                return 0;
        }, static_cast<void*>(&ret), nullptr);

        return ret;
    };

private:
    ::sqlite3* handle_;

};

} // namespace db
