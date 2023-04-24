#include "UserService.hh"

namespace server::service {

    oatpp::Object<server::dto::UserDto>
    UserService::createOne(
            oatpp::Object<dto::UserDto> const &dto
    ) {
        OATPP_ASSERT_HTTP(
                dto->email && dto->username && dto->password,
                Status::CODE_400,
                "REQUIRED_PARAMETER_NOT_PROVIDED"
        )

        OATPP_ASSERT_HTTP(
                database->executeQuery(
                        "SELECT * FROM users WHERE email = :user.email",
                        {{"user", dto}}
                )->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>()->empty(),
                Status::CODE_409,
                "EMAIL_ALREADY_EXISTS"
        )

        OATPP_ASSERT_HTTP(
                database->executeQuery(
                        "SELECT * FROM users WHERE username = :user.username",
                        {{"user", dto}}
                )->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>()->empty(),
                Status::CODE_409,
                "USERNAME_ALREADY_EXISTS"
        )

        auto queryResult = database->createUser(dto);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage());

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>();

        OATPP_ASSERT_HTTP(!fetchResult->empty(), Status::CODE_500, "NO_ROWS_CREATED");
        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "UNEXPECTED_NUMBER_OF_ROWS_CREATED");

        return fetchResult[0];
    }

    oatpp::Object<dto::UserDto>
    UserService::getOne(
            oatpp::UInt64 const &userId
    ) {
        OATPP_ASSERT_HTTP(userId, Status::CODE_400, "INVALID_USER_ID");


        auto queryResult = database->getUserById(userId);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage());
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_404, "USER_NOT_FOUND");

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>();

        OATPP_ASSERT_HTTP(!fetchResult->empty(), Status::CODE_500, "NO_ROWS_FETCHED");
        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "UNEXPECTED_NUMBER_OF_ROWS_FETCHED");

        return fetchResult[0];
    }

    oatpp::Object<dto::PageDto<dto::UserPageDto>>
    UserService::searchUsers(
            oatpp:: const &query,
            oatpp::UInt64 const &limit,
            oatpp::UInt64 const &offset
    ) {
        std::string query = "SELECT * FROM users";


        if (username) {
            query += "username LIKE :username";
        }

        query += " LIMIT :limit OFFSET :offset";

        auto queryResult = database->executeQuery(

                {
                        {"username",},
                        {"limit",  limit},
                        {"offset", offset}
                }
        );

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