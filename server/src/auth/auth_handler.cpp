
#include "auth/auth_handler.hpp"

auth_handler::auth_handler(std::shared_ptr<JWT> jwt)
  : oatpp::web::server::handler::BearerAuthorizationHandler("API" /* Realm */)
  , jwt_(std::move(jwt))
{}

std::shared_ptr<auth_handler::AuthorizationObject> auth_handler::authorize(const oatpp::String& token) {
  return jwt_->readAndVerifyToken(token);
}
