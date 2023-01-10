#include "database/context.hh"
#include "database/exceptions.hh"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace db
{

context::context(std::filesystem::path const& path, int flags) :
    handle_{nullptr}
{
    auto res = sqlite3_open_v2(
#ifdef _MSC_VER
        path.string().c_str(),
#else
        path.c_str(),
#endif
        &handle_,
        flags,
        nullptr
    );
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

// TODO remove
//[[nodiscard]]
//nlohmann::json
//context::query(char const* query)
//{
//    nlohmann::json ret{};
//
//    this->raw_query(
//        query,
//        [](void* user_data, int col_count, char** col_text, char** col_name){
//            nlohmann::json& json = *static_cast<nlohmann::json*>(user_data);
//            for(int col_i=0;col_i<col_count;++col_i) {
//                // Check if number
//                char* endptr = nullptr;
//                auto ret = std::strtol(col_text[col_i], &endptr, 10);
//                if(endptr == col_text[col_i]) {
//                    json[col_name[col_i]].push_back(col_text[col_i]);
//                } else {
//                    json[col_name[col_i]].push_back(ret);
//                }
//            }
//            return SQLITE_OK;
//        },
//        static_cast<void*>(&ret)
//    );
//
//    return ret;
//}

[[nodiscard]]
db::statement
context::prepare_statement(char const* str) noexcept
{
    return db::statement{handle_, str};
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
