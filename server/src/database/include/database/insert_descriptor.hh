#pragma once

namespace db
{

/**
 * @brief Describes a structure which can be inserted to an SQL table.
 * Example usage:
 * @code
template<>
struct ::db::insert_descriptor<user>
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
        std::pair{&user_test::name,     "name"},
        std::pair{&user_test::password, "password"},
    };
};
 * @endcode
 */
template<class Type>
struct insert_descriptor;

} // namespace db
