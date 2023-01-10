#include "database/statement.hh"

#include "database/exceptions.hh"

#include <cstring>
#include <utility>

namespace db
{

statement::statement(::sqlite3* database, char const* str) noexcept :
    handle_{nullptr}
{
    ::sqlite3_prepare_v3(database, str, static_cast<int>(::strlen(str)+1), 0, &handle_, nullptr);
}

statement::~statement()
{
    ::sqlite3_finalize(handle_);
}

statement::statement(statement&& other) noexcept :
    handle_(std::exchange(other.handle_, nullptr)) {}

statement&
statement::operator=(statement&& other) noexcept
{
    handle_ = std::exchange(other.handle_, nullptr);
    return *this;
}

void
statement::exec()
{
    for(;;) {
        int ret = ::sqlite3_step(handle_);
        if(ret == SQLITE_DONE)   { return; }
        if(ret == SQLITE_MISUSE) { throw db::database_misuse{}; }
        if(ret == SQLITE_ERROR)  { throw db::database_query_error{}; }
    }
}

nlohmann::json
statement::exec_json()
{
    nlohmann::json json{};

    for(;;) {
        int ret = ::sqlite3_step(handle_);
        if(ret == SQLITE_ROW) {
            int col_count = ::sqlite3_column_count(handle_);
            for(int col_i=0;col_i<col_count;++col_i) {
                auto col_type = ::sqlite3_column_type(handle_, col_i);
                if(col_type == SQLITE_INTEGER) {
                    json[::sqlite3_column_name(handle_, col_i)]
                        .push_back(::sqlite3_column_int(handle_, col_i));
                } else if(col_type == SQLITE_FLOAT) {
                    json[::sqlite3_column_name(handle_, col_i)]
                        .push_back(::sqlite3_column_double(handle_, col_i));
                } else if(col_type == SQLITE_NULL) {
                    json[::sqlite3_column_name(handle_, col_i)]
                        .push_back("*NULL*");
                } else if(col_type == SQLITE_TEXT) {
                    json[::sqlite3_column_name(handle_, col_i)]
                        .push_back(reinterpret_cast<char const*>(::sqlite3_column_text(handle_, col_i)));
                }
            }
        }
        else if(ret == SQLITE_DONE)   { break; }
        else if(ret == SQLITE_MISUSE) { throw db::database_misuse{}; }
        else if(ret == SQLITE_ERROR)  { throw db::database_query_error{}; }
    }

    return json;
}

void
statement::clear_bindings() noexcept
{
    ::sqlite3_clear_bindings(handle_);
}

void
statement::reset() noexcept
{
    ::sqlite3_reset(handle_);
}

template<>
void
statement::bind(int index, double value) noexcept
{
    ::sqlite3_bind_double(handle_, index, value);
}

template<>
void
statement::bind(int index, int value) noexcept
{
    ::sqlite3_bind_int(handle_, index, value);
}

template<>
void
statement::bind(int index, char const* value) noexcept
{
    ::sqlite3_bind_text(handle_, index, value, -1, nullptr);
}

} // namespace db
