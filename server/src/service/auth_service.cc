#include "service/auth_service.hh"
#include <oatpp/core/Types.hpp>

//NOLINTNEXTLINE
using Status = ::oatpp::web::protocol::http::Status;

namespace server::service
{

::oatpp::Object<::server::dto::auth_dto> auth_service::signUp(::oatpp::Object<::server::dto::signUp_dto> const& dto) {

        //Is already exist user with same username and email
        auto res = database_->is_exist(dto->email,dto->username);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(!res->hasMoreToFetch(), Status::CODE_409, "User already exists");
    
        //Adding user to database
        res = database_->signUp(dto);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(!res->hasMoreToFetch(), Status::CODE_404, "User not Created");
        
        ::oatpp::Object<::server::dto::signIn_dto> user = std::make_shared<dto::signIn_dto>();
        user->password = dto->password;
        user->email = dto->email;
        
        return signIn(user);
    }

    ::oatpp::Object<::server::dto::auth_dto> auth_service::signIn(::oatpp::Object<::server::dto::signIn_dto> const& dto) {

        auto res = database_->signIn(dto->email,dto->password);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(res->hasMoreToFetch(), Status::CODE_404, "User not found");

        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::user_dto>>>();
        OATPP_ASSERT_HTTP(fetch->size() == 1, Status::CODE_500, "Unknown error");

        //Generating tokn
        auto payload = std::make_shared<JWT::Payload>();
        payload->user = fetch[0];
        auto auth = ::server::dto::auth_dto::createShared();
        auth->token = jwt_->createToken(payload);
        auth->admin = fetch[0]->admin;

        return auth;
    }

} // namespace server::service

