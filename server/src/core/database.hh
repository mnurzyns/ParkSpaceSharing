#pragma once

#include <tuple>

#include <database/descriptor.hh>

#include "core.hpp"

template<>
struct db::descriptor<User>
{
    static
    constexpr
    auto
    get()
    {
        return std::pair{
            ::std::tuple{
                ::std::pair{&User::id, "id"},
                ::std::pair{&User::username, "username"},
                ::std::pair{&User::password, "password"},
            },
            "user"
        };
    }
};
