#include "UserService.hh"

#include "Validator.hh"

namespace server::service {

std::shared_ptr<UserService>
UserService::createShared()
{
    return std::make_shared<UserService>();
}

Object<UserDto>
UserService::createOne(Object<UserDto> const& dto)
{
    ValidateEmailHTTP(dto->email);

    ValidatePhoneHTTP(dto->phone);

    try {
        this->getOne(dto->id); // Will throw 404 if not found
        OATPP_ASSERT_HTTP(false, Status::CODE_409, "User already exists")
    } catch (oatpp::web::protocol::http::HttpError& e) {
        if (e.getInfo().status != Status::CODE_404) {
            throw;
        }
    }

    auto query_result = database_->createUser(dto);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    OATPP_ASSERT_HTTP(
      query_result->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

    auto fetch_result = query_result->fetch<Vector<Object<UserDto>>>();

    OATPP_ASSERT_HTTP(fetch_result->size() == 1,
                      Status::CODE_500,
                      "Unexpected number of rows returned!")

    return fetch_result[0];
}

Object<UserDto>
UserService::getOne(UInt64 const& id)
{
    auto query_result = database_->getUser(id);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    OATPP_ASSERT_HTTP(
      query_result->hasMoreToFetch(), Status::CODE_404, "User not found")

    auto fetch_result = query_result->fetch<Vector<Object<UserDto>>>();

    OATPP_ASSERT_HTTP(fetch_result->size() == 1,
                      Status::CODE_500,
                      "Unexpected number of rows returned!")

    // TODO(papaj-na-wrotkach): use polymorphic DTOs instead
    fetch_result[0]->password = nullptr;

    return fetch_result[0];
}

Object<UserPageDto>
UserService::search(String const& query,
                    UInt64 const& limit,
                    UInt64 const& offset)
{
    char const* query_table_fts = " FROM user_fts";
    char const* query_filters =
      query->empty() ? "" : " WHERE user_fts MATCH :query";

    auto query_total_result = database_->executeQuery(
      std::string{} + "SELECT COUNT(*)" + query_table_fts + query_filters + ";",
      { { "query", String(query) } });

    OATPP_ASSERT_HTTP(query_total_result->isSuccess(),
                      Status::CODE_500,
                      query_total_result->getErrorMessage())

    auto fetch_total_result =
      query_total_result->fetch<Vector<Vector<UInt64>>>();

    OATPP_ASSERT_HTTP(
      fetch_total_result[0][0] > 0, Status::CODE_404, "No users found")

    auto query_result = database_->executeQuery(
      std::string{} + "SELECT user.*" + query_table_fts +
        " INNER JOIN user ON user.id = user_fts.user_id" + query_filters +
        " LIMIT :offset,:limit;",
      { { "query", String(query) },
        { "offset", UInt64(offset) },
        { "limit", UInt64(limit) } });

    auto fetch_result = query_result->fetch<Vector<Object<UserDto>>>();

    auto page = UserPageDto::createShared();
    page->items = fetch_result;
    page->limit = limit;
    page->offset = offset;
    page->count = fetch_total_result[0][0];

    // TODO(papaj-na-wrotkach): use polymorphic DTOs instead
    for (auto& user : *page->items) {
        user->password = nullptr;
    }

    return page;
}

Object<UserDto>
UserService::putOne(Object<UserDto> const& dto)
{
    ValidateEmailHTTP(dto->email);
    ValidatePhoneHTTP(dto->phone);

    auto query_result = database_->replaceUser(dto);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    OATPP_ASSERT_HTTP(
      query_result->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

    auto fetch_result = query_result->fetch<Vector<Object<UserDto>>>();

    OATPP_ASSERT_HTTP(fetch_result->size() == 1,
                      Status::CODE_500,
                      "Unexpected number of rows returned!")

    return fetch_result[0];
}

Object<UserDto>
UserService::patchOne(UInt64 const& id, Object<UserDto> const& dto)
{
    if (dto->email) {
        ValidateEmailHTTP(dto->email);
    }

    if (dto->phone) {
        ValidatePhoneHTTP(dto->phone);
    }

    auto existing = this->getOne(id);

    existing->id = dto->id ? dto->id : existing->id;
    existing->email = dto->email ? dto->email : existing->email;
    existing->phone = dto->phone ? dto->phone : existing->phone;
    existing->username = dto->username ? dto->username : existing->username;
    existing->password = dto->password ? dto->password : existing->password;
    existing->role = dto->role ? dto->role : existing->role;

    return this->putOne(existing);
}

Object<StatusDto>
UserService::deleteOne(UInt64 const& id)
{
    this->getOne(id); // Will throw 404 if not found

    auto query_result = database_->deleteUser(id);

    OATPP_ASSERT_HTTP(query_result->isSuccess(),
                      Status::CODE_500,
                      query_result->getErrorMessage())

    auto status = StatusDto::createShared();
    status->status = "OK";
    status->code = Status::CODE_200.code;
    status->message = "User deleted successfully";

    return status;
}

} // namespace server::service
