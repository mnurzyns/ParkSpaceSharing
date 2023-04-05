#ifndef auth_handler_HPP
#define auth_handler_HPP

#include "auth/JWT.hpp"

class auth_handler : public oatpp::web::server::handler::BearerAuthorizationHandler {
private:
  std::shared_ptr<JWT> jwt_;
public:

  explicit auth_handler(std::shared_ptr<JWT> jwt);

  std::shared_ptr<AuthorizationObject> authorize(const oatpp::String& token) override;

};

#endif //auth_handler_HPP
