#include "UserService.hh"

namespace server::service {

oatpp::Object<dto::UserDto> UserService::createOne(oatpp::Object<database::model::UserModel> const& model) {
    OATPP_ASSERT_HTTP(model->email, Status::CODE_400, "EMAIL_NOT_PROVIDED");
    OATPP_ASSERT_HTTP(model->username, Status::CODE_400, "USERNAME_NOT_PROVIDED");
    OATPP_ASSERT_HTTP(model->password, Status::CODE_400, "PASSWORD_NOT_PROVIDED");

    OATPP_ASSERT_HTTP(
        database->getUserByEmail(model->email)->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>()->empty(),
        Status::CODE_409,
        "EMAIL_ALREADY_EXISTS"
    );

    OATPP_ASSERT_HTTP(
        database->getUserByUsername(model->username)->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>()->empty(),
        Status::CODE_409,
        "USERNAME_ALREADY_EXISTS"
    );

    auto queryResult = database->createOneUser(model);

    OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage());

    auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>();

    OATPP_ASSERT_HTTP(!fetchResult->empty(), Status::CODE_500, "NO_ROWS_CREATED");
    OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "UNEXPECTED_NUMBER_OF_ROWS_CREATED");

    return fetchResult[0];
}

oatpp::Object<dto::UserDto> UserService::getUser(oatpp::Int64 const& userId) {
    OATPP_ASSERT_HTTP(userId, Status::CODE_400, "INVALID_USER_ID");


    auto queryResult = database->getUser(userId);

    OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage());
    OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_404, "USER_NOT_FOUND");

    auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>();

    OATPP_ASSERT_HTTP(!fetchResult->empty(), Status::CODE_500, "NO_ROWS_FETCHED");
    OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "UNEXPECTED_NUMBER_OF_ROWS_FETCHED");

    return fetchResult[0];
}

oatpp::Object<dto::UserSearchPageDto> UserService::searchUsers(oatpp::String const& query, oatpp::UInt64 const& limit, oatpp::UInt64 const& offset) {
    auto queryResult = database->searchUsers(query, limit, offset);

    OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage());

    auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>();

    auto page = dto::UserSearchPageDto::createShared();
    page->limit = limit;
    page->offset = offset;
    page->total = fetchResult->size();
    page->content = fetchResult;

    return page;
}

}