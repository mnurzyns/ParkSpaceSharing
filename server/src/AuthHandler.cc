#include "AuthHandler.hh"

namespace server {

AuthHandler::AuthHandler(std::shared_ptr<TokenUtils> token_utils)
  : oatpp::web::server::handler::BearerAuthorizationHandler("API")
  , token_utils_(std::move(token_utils))
{
}

std::shared_ptr<AuthHandler::AuthorizationObject>
AuthHandler::authorize(String const& token)
{
    auto decoded = jwt::decode(token);
    token_utils_->verifyToken(decoded);
    return std::make_shared<TokenPayload>(TokenUtils::readToken(decoded));
}

} // namespace server
