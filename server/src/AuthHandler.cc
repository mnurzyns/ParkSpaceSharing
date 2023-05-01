#include "AuthHandler.hh"
#include "JWT.hh"

namespace server {

AuthHandler::AuthHandler(std::shared_ptr<JWT> jwt_object)
  : oatpp::web::server::handler::BearerAuthorizationHandler("API")
  , jwt_object_(std::move(jwt_object))
{
}

std::shared_ptr<AuthHandler::AuthorizationObject>
AuthHandler::authorize(String const& token)
{
    return jwt_object_->readAndVerifyToken(token);
}

} // namespace server