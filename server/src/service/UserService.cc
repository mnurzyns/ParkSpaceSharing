#include "UserService.hh"

namespace server::service {

std::shared_ptr<UserService>
UserService::createShared()
{
    return std::make_shared<UserService>();
}

Object<UserDto>
UserService::createOne(Object<UserDto> const& dto)
{
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

    return fetch_result[0];
}

Object<UserPageDto>
UserService::search(String const& query,
                    UInt64 const& limit,
                    UInt64 const& offset)
{
    std::string const queryTableFts = " FROM user_fts";
    std::string const queryFilters =
      query->empty() ? std::string{} : " WHERE user_fts MATCH :query";

    auto queryTotalResult = database_->executeQuery(
      "SELECT COUNT(*)" + queryTableFts + queryFilters + ";",
      { { "query", String(query) } });

    OATPP_ASSERT_HTTP(queryTotalResult->isSuccess(),
                      Status::CODE_500,
                      queryTotalResult->getErrorMessage())

    auto fetchTotalResult = queryTotalResult->fetch<Vector<Vector<UInt64>>>();

    OATPP_ASSERT_HTTP(
      fetchTotalResult[0][0] > 0, Status::CODE_404, "No users found")

    auto query_result = database_->executeQuery(
      "SELECT user.*" + queryTableFts +
        " INNER JOIN user ON user.id = user_fts.user_id" + queryFilters +
        " LIMIT :offset,:limit;",
      { { "query", String(query) },
        { "offset", UInt64(offset) },
        { "limit", UInt64(limit) } });

    auto fetch_result = query_result->fetch<Vector<Object<UserDto>>>();

    auto page = UserPageDto::createShared();
    page->items = fetch_result;
    page->limit = limit;
    page->offset = offset;
    page->count = fetchTotalResult[0][0];

    return page;
}

Object<UserDto>
UserService::putOne(Object<UserDto> const& dto)
{
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
    auto existing = this->getOne(id);

    existing->id = dto->id ? dto->id : existing->id;
    existing->email = dto->email ? dto->email : existing->email;
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
    status->message = "User was successfully deleted";
    return status;
}

} // namespace server::service
