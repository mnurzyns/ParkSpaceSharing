#include "database/context.hh"

#include <cstdio>
#include <stdexcept>

namespace db
{

context::context(std::filesystem::path const& path) :
    handle_{}
{
    auto res = sqlite3_open_v2(path.c_str(), &handle_, SQLITE_OPEN_READWRITE, nullptr);
    if(res != SQLITE_OK) {
        std::fprintf(stderr, "%s\n", ::sqlite3_errmsg(handle_));
        sqlite3_close_v2(handle_);
        throw std::runtime_error{"Failed to open sqlite3 database."};
    }
}

context::~context()
{
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
        std::fprintf(stderr, "%s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close_v2(handle_);
        // Failed query is considered a bug and should not happen;
        throw std::runtime_error{"Raw query failed."};
    }
}

} // namespace db
