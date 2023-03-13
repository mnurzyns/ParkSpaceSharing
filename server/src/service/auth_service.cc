#include "service/auth_service.hh"
#include <oatpp-sqlite/Utils.hpp>
#include <oatpp/web/protocol/http/Http.hpp>
#include "dto/signIn_dto.hh"
#include "dto/signUp_dto.hh"
#include "dto/auth_dto.hh"

//NOLINTNEXTLINE
using Status = ::oatpp::web::protocol::http::Status;

namespace server::service
{

    ::oatpp::Object<::server::dto::auth_dto> auth_service::signUp(::oatpp::Object<::server::dto::signUp_dto> const& dto) {

        
        auto payload = std::make_shared<JWT::Payload>();
        payload->userId = dto->username;

        ::oatpp::String tok = jwt_->createToken(payload);

        auto res = database_->signUp(dto,tok);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(res->hasMoreToFetch(), Status::CODE_404, "User not Created");

        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::auth_dto>>>();
        OATPP_ASSERT_HTTP(fetch->size() > 1, Status::CODE_500, "Unknown error");

        return fetch[0];
    }

    ::oatpp::Object<::server::dto::auth_dto> auth_service::signIn(::oatpp::Object<::server::dto::signIn_dto> const& dto) {

        auto res = database_->signIn_auth(dto);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(res->hasMoreToFetch(), Status::CODE_404, "User not found");

        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::auth_dto>>>();
        OATPP_ASSERT_HTTP(fetch->size() > 1, Status::CODE_500, "Unknown error");

        return fetch[0];
    }

} // namespace server::service

