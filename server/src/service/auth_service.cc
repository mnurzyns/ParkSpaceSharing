#include "service/auth_service.hh"


//NOLINTNEXTLINE
using Status = ::oatpp::web::protocol::http::Status;

namespace server::service
{

    ::oatpp::Object<::server::dto::auth_dto> auth_service::signUp(::oatpp::Object<::server::dto::signUp_dto> const& dto) {

        //Is already exist user with same username and email
        auto res = database_->is_exist(dto->email,dto->username);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        
        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::auth_dto>>>();//powinno być UInt32
        OATPP_ASSERT_HTTP(fetch->size() == 0, Status::CODE_409, "User already exists");
    
        //Generating tokn
        //auto payload = std::make_shared<JWT::Payload>();
        //payload->userId = dto->password;

        auto auth = ::server::dto::auth_dto::createShared();
        //auth->token = jwt_->createToken(payload);

        auth->token = "token";

        //Adding user to database
        res = database_->signUp(dto,auth->token);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(!res->hasMoreToFetch(), Status::CODE_404, "User not Created");

        return auth;
    }

    ::oatpp::Object<::server::dto::auth_dto> auth_service::signIn(::oatpp::Object<::server::dto::signIn_dto> const& dto) {

        auto res = database_->signIn_auth(dto);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(res->hasMoreToFetch(), Status::CODE_404, "User not found");

        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::auth_dto>>>();
        OATPP_ASSERT_HTTP(fetch->size() == 1, Status::CODE_500, "Unknown error");

        return fetch[0];
    }

} // namespace server::service

