#include "UserService.hh"

namespace server::service {

    oatpp::Object<server::dto::UserDto>
    UserService::createOne(
            oatpp::Object<server::dto::UserDto> const &dto
    ) {
        try {
            this->getOne(dto->id); // Will throw 404 if not found
            OATPP_ASSERT_HTTP(false, Status::CODE_409, "User already exists")
        }
        catch (oatpp::web::protocol::http::HttpError &e) {
            if (e.getInfo().status != Status::CODE_404) { throw; }
        }

        auto queryResult = database_->createUser(dto);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<server::dto::UserDto>
    UserService::getOne(
            oatpp::UInt64 const &id
    ) {
        auto queryResult = database_->getUser(id);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_404, "Not found")

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<dto::UserPageDto>
    UserService::search(
            oatpp::String const &query,
            oatpp::UInt64 const &limit,
            oatpp::UInt64 const &offset
    ) {
        std::string const queryTableFts = " FROM user_fts";
        std::string const queryFilters = query->empty() ? std::string{} : " WHERE user_fts MATCH :query";

        auto queryTotalResult = database_->executeQuery(
                "SELECT COUNT(*)" + queryTableFts + queryFilters + ";",
                {{"query", oatpp::String(query)}}
        );

        OATPP_ASSERT_HTTP(queryTotalResult->isSuccess(), Status::CODE_500, queryTotalResult->getErrorMessage())

        auto fetchTotalResult = queryTotalResult->fetch<oatpp::Vector<oatpp::Vector<oatpp::UInt64>>>();

        OATPP_ASSERT_HTTP(fetchTotalResult[0][0] > 0, Status::CODE_404, "Not found")

        auto queryResult = database_->executeQuery(
                "SELECT user.*" + queryTableFts +
                " INNER JOIN user ON user.id = user_fts.user_id" + queryFilters +
                " LIMIT :offset,:limit;", {
                        {"query",  oatpp::String(query)},
                        {"offset", oatpp::UInt64(offset)},
                        {"limit",  oatpp::UInt64(limit)}
                });

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>();

        auto page = dto::UserPageDto::createShared();
        page->items = fetchResult;
        page->limit = limit;
        page->offset = offset;
        page->count = fetchTotalResult[0][0];

        return page;
    }

    oatpp::Object<dto::UserDto>
    UserService::putOne(
            oatpp::Object<dto::UserDto> const &dto
    ) {
        auto queryResult = database_->replaceUser(dto);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())
        OATPP_ASSERT_HTTP(queryResult->hasMoreToFetch(), Status::CODE_500, "No rows returned!")

        auto fetchResult = queryResult->fetch<oatpp::Vector<oatpp::Object<dto::UserDto>>>();

        OATPP_ASSERT_HTTP(fetchResult->size() == 1, Status::CODE_500, "Unexpected number of rows returned!")

        return fetchResult[0];
    }

    oatpp::Object<dto::UserDto>
    UserService::patchOne(
            oatpp::UInt64 const &id,
            oatpp::Object<dto::UserDto> const &dto
    ) {
        auto existing = this->getOne(id);

        existing->email = dto->email ? dto->email : existing->email;
        existing->id = dto->id ? dto->id : existing->id;
        existing->password = dto->password ? dto->password : existing->password;
        existing->role = dto->role ? dto->role : existing->role;
        existing->username = dto->username ? dto->username : existing->username;

        return this->putOne(existing);
    }

    oatpp::Object<dto::StatusDto>
    UserService::deleteOne(
            const oatpp::UInt64 &id
    ) {
        this->getOne(id); // Will throw 404 if not found

        auto queryResult = database_->deleteUser(id);

        OATPP_ASSERT_HTTP(queryResult->isSuccess(), Status::CODE_500, queryResult->getErrorMessage())

        auto status = dto::StatusDto::createShared();
        status->status = "OK";
        status->code = Status::CODE_200.code;
        status->message = "User was successfully deleted";
        return status;
    }

}  // namespace server::service
