#pragma once

#include <string>

#include <sqlite3.h>

#include <nlohmann/json.hpp>

namespace db
{

namespace detail
{

template<class Type>
concept has_c_str = requires(Type&& type)
{
    { type.c_str() } -> std::same_as<char const*>;
};

};

class context;

/**
 * @brief Represents a query statement.
 */
class statement
{
public:
    ~statement();

    statement(statement const&) = delete;

    statement&
    operator=(statement const&) = delete;

    [[nodiscard]]
    statement(statement&&) noexcept;

    [[nodiscard]]
    statement&
    operator=(statement&&) noexcept;

    /**
     * @brief Bind a value to a parameter.
     *
     * @param index Index of the parameter, left most = 1
     */
    template<class T>
    requires(!detail::has_c_str<T>)
    void
    bind(int index, T value) noexcept;

    template<detail::has_c_str T>
    void
    bind(int index, T const& value) noexcept
    {
        ::sqlite3_bind_text(handle_, index, value.c_str(), -1, nullptr);
    }

    /**
     * @brief Execute the statement(s), ignore output.
     *
     * @throws db::database_misuse
     * @throws db::database_query_error
     */
    void
    exec();

    /**
     * @brief Execute the statement(s), return output as json.
     *
     * @throws db::database_misuse
     * @throws db::database_query_error
     */
    [[nodiscard]]
    nlohmann::json
    exec_json();

    /**
     * @brief Clear bindings.
     */
    void
    clear_bindings() noexcept;

    /**
     * @brief Reset the statement and re-use it.
     */
    void
    reset() noexcept;

protected:
    friend class ::db::context;
    statement(::sqlite3* database, char const* str) noexcept;

private:
    ::sqlite3_stmt* handle_;
};

#define INST(TYPE) \
    template<> \
    void \
    statement::bind<TYPE>(int, TYPE) noexcept;

INST(double)
INST(int)
INST(char const*)

#undef INST

} // namespace db
