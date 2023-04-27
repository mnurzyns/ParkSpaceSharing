#pragma once

#include <oatpp/core/macro/component.hpp>

#include "auth/JWT.hh"
#include "database/MainDatabase.hh"
#include "dto/AuthDto.hh"
#include "dto/SignInDto.hh"
#include "dto/SignUpDto.hh"

namespace server::service {

    using Status = oatpp::web::protocol::http::Status;

    class AuthService {
    private:
        OATPP_COMPONENT(std::shared_ptr<database::MainDatabase>, database_);
        OATPP_COMPONENT(std::shared_ptr<auth::JWT>, jwt_);

    public:
        oatpp::Object<dto::AuthDto>
        signIn(
                oatpp::Object<dto::SignInDto> const &dto
        );

        oatpp::Object<server::dto::AuthDto>
        signUp(
                oatpp::Object<dto::SignUpDto> const &dto
        );

    };

} // namespace server::service

