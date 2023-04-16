#ifndef AUTHINTERCEPTOR_HPP
#define AUTHINTERCEPTOR_HPP

#include "auth/auth_handler.hpp"

#include <oatpp/web/server/interceptor/RequestInterceptor.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

class AuthInterceptor : public oatpp::web::server::interceptor::RequestInterceptor {
private:
  AuthHandler m_auth_handler;
  oatpp::web::server::HttpRouterTemplate<bool> authEndpoints;
  oatpp::web::server::HttpRouterTemplate<bool> adminEndpoints;
public:

  AuthInterceptor(const std::shared_ptr<JWT>& jwt);

  std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest>& request) override;
};

#endif //AUTHINTERCEPTOR_HPP