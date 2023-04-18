#include "AuthHandler.hh"

namespace server::auth {

AuthHandler::AuthHandler(const std::shared_ptr<JWT> &jwt)
    : oatpp::web::server::handler::BearerAuthorizationHandler("API" /* Realm */), jwt_(jwt) {}

std::shared_ptr<AuthHandler::AuthorizationObject> AuthHandler::authorize(const oatpp::String &token) {
    return jwt_->readAndVerifyToken(token);
}

}  // namespace server::auth