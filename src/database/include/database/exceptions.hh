#pragma once

#include <stdexcept>

namespace db
{

class database_open_error :
    public virtual std::runtime_error
{
public:
    [[nodiscard]]
    database_open_error() :
        std::runtime_error{"Failed to open sqlite database"} {}
};

class database_query_error :
    public virtual std::runtime_error
{
public:
    [[nodiscard]]
    database_query_error() :
        std::runtime_error{"Failed to query sqlite database"} {}
};

class database_misuse :
    public virtual std::runtime_error
{
public:
    [[nodiscard]]
    database_misuse() :
        std::runtime_error{"Database missuse"} {}
};

} // namespace db
