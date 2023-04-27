#include "AuthHandler.hh"

namespace server::auth {

    AuthHandler::AuthHandler(std::shared_ptr<JWT> const &jwt)
            : oatpp::web::server::handler::BearerAuthorizationHandler("API"), jwt_(jwt) {}

    std::shared_ptr<AuthHandler::AuthorizationObject>
    AuthHandler::authorize(
            oatpp::String const &token
    ) {
        return jwt_->readAndVerifyToken(token);
    }

}  // namespace server::auth