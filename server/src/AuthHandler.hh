#pragma once

#include <oatpp/web/server/handler/AuthorizationHandler.hpp>

#include "TokenUtils.hh"

namespace server {

using oatpp::String;

class AuthHandler
  : public oatpp::web::server::handler::BearerAuthorizationHandler
{
  private:
    std::shared_ptr<TokenUtils> token_utils_;

  public:
    [[nodiscard]] explicit AuthHandler(std::shared_ptr<TokenUtils> token_utils);

    std::shared_ptr<AuthorizationObject>
    authorize(String const& token) override;
};

} // namespace server
