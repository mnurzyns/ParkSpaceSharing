#pragma once

#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>

#include "database/insert_descriptor.hh"
#include "database/statement.hh"

namespace db::detail
{

template<class Type>
[[nodiscard]]
std::string
prepare_insert_string()
{
    using type_t = std::decay_t<Type>;
    using desc_t = db::insert_descriptor<type_t>;
    static constexpr auto COLUMNS =
        []<class... Args>(std::tuple<Args...> const& tup) consteval
        {
            std::array<char const*, sizeof...(Args)> columns{};
            std::size_t columns_i=0;

            std::apply([&](auto&&... args) {
            (..., [&](auto&& arg){
                columns[columns_i++] = arg.second;
            }(args));
            }, tup);

            return columns;
        }(desc_t::COLUMNS);

    // Build query

    std::stringstream sstr{};
    sstr
        << "INSERT INTO "
        << '`' << desc_t::TABLE << '`'
        << " (";
    for(bool first = true; auto name : COLUMNS) {
        if(!first) { sstr << ','; }
        first = false;
        sstr << '`' << name << '`';
    }
    sstr << ')';

    // INSERT INTO `table` (`column0`, ...)

    sstr << " VALUES (";
    for(std::size_t i=0;i<COLUMNS.size();++i) {
        if(i != 0) { sstr << ','; }
        sstr << '?';
    }
    sstr << ");";

    // INSERT INTO `table` (`column0`, ...) VALUES (?, ...);

    return sstr.str();
}

template<class Type>
void
prepare_insert_bind(::db::statement& statement, int& arg_i, Type&& value)
{
    using type_t = std::decay_t<Type>;
    using desc_t = db::insert_descriptor<type_t>;

    std::apply([&](auto&&... args){
        (..., [&](auto&& arg){
            statement.bind(arg_i++, value.*arg.first);
        }(args)); }, desc_t::COLUMNS);
}

} // namespace db::detail
