
#include "auth/auth_handler.hpp"

auth_handler::auth_handler(std::shared_ptr<JWT> jwt)
  : oatpp::web::server::handler::BearerAuthorizationHandler("API" /* Realm */)
  , m_jwt_(std::move(jwt))
{}

std::shared_ptr<auth_handler::AuthorizationObject> auth_handler::authorize(const oatpp::String& token) {
  return m_jwt->readAndVerifyToken(token);
}
