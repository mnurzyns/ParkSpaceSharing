#pragma once

#include <oatpp/web/server/handler/AuthorizationHandler.hpp>

#include "JWT.hh"

namespace server {

using oatpp::String;

class AuthHandler
  : public oatpp::web::server::handler::BearerAuthorizationHandler
{
  private:
    std::shared_ptr<JWT> jwt_object_;

  public:
    [[nodiscard]] explicit AuthHandler(std::shared_ptr<JWT> jwt_object);

    std::shared_ptr<AuthorizationObject>
    authorize(String const& token) override;
};

} // namespace server
