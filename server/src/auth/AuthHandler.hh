#pragma once

#include <oatpp/web/server/handler/AuthorizationHandler.hpp>

#include "JWT.hh"

namespace server::auth {

    class AuthHandler : public oatpp::web::server::handler::BearerAuthorizationHandler {

    private:
        std::shared_ptr<JWT> jwt_;
    public:
        [[nodiscard]]
        explicit
        AuthHandler(
                std::shared_ptr<JWT> jwt
        );

        std::shared_ptr<AuthorizationObject>
        authorize(
                oatpp::String const &token
        ) override;
    };

}  // namespace server::auth
