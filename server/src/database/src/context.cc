#include "database/context.hh"
#include "database/exceptions.hh"

#include <iostream>
#include <stdexcept>
#include <utility>

namespace db
{

context::context(std::filesystem::path const& path) :
    handle_{nullptr}
{
    auto res = sqlite3_open_v2(path.c_str(), &handle_, SQLITE_OPEN_READWRITE, nullptr);
    if(res != SQLITE_OK) {
        std::cerr << ::sqlite3_errmsg(handle_) << '\n';
        throw db::database_open_error{};
    }
}

context::context(context&& other) noexcept :
    handle_{std::exchange(other.handle_, nullptr)} {}

context&
context::operator=(context&& other) noexcept
{
    handle_ = std::exchange(other.handle_, nullptr);
    return *this;
}

context::~context()
{
    // Calling this function with a
    // nullptr is a valid no-op
    ::sqlite3_close_v2(handle_);
}

void
context::raw_query(
    char const* query,
    int(*callback)(void*,int,char**,char**),
    void* user_data
)
{
    char* err_msg = nullptr;
    auto res = ::sqlite3_exec(handle_, query, callback, user_data, &err_msg);
    if(res != SQLITE_OK) {
        std::cerr << err_msg << '\n';
        sqlite3_free(err_msg);
        // Failed query is considered a bug and should not happen;
        throw db::database_query_error{};
    }
}

} // namespace db
