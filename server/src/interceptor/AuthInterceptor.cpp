
#include "AuthInterceptor.hpp"
#include "auth/JWT.hpp"

AuthInterceptor::AuthInterceptor(const std::shared_ptr<JWT>& jwt)
  : m_auth_handler(jwt)
{
  // endpoints without token authentication
  authEndpoints.route("POST", "users/signup", false);
  authEndpoints.route("POST", "users/signin", false);
  authEndpoints.route("GET", "offers", false);
  authEndpoints.route("GET", "swagger/*", false);
  authEndpoints.route("GET", "api-docs/oas-3.0.0.json", false);

  // endpoints with for admin authentication
  adminEndpoints.route("GET", "users/*", false);
  adminEndpoints.route("POST", "users/*", false);
  adminEndpoints.route("DELETE", "users/*", false);
  
}

std::shared_ptr<AuthInterceptor::OutgoingResponse> AuthInterceptor::intercept(const std::shared_ptr<IncomingRequest>& request) {

  auto r = authEndpoints.getRoute(request->getStartingLine().method, request->getStartingLine().path);
  if(r && !r.getEndpoint()) {
    return nullptr; // Continue without Authorization
  }

  auto authHeader = request->getHeader(oatpp::web::protocol::http::Header::AUTHORIZATION);
  auto authObject = std::static_pointer_cast<JWT::payload>(m_auth_handler.handleAuthorization(authHeader));
  auto admin_r = adminEndpoints.getRoute(request->getStartingLine().method, request->getStartingLine().path);

  if(authObject) {
    request->putBundleData("userId", authObject->userId);


    if(admin_r && !admin_r.getEndpoint()) {
      if(authObject->isAdmin){
        return nullptr;
      }
    } else {
      return nullptr;
    }    
  }

  throw oatpp::web::protocol::http::HttpError(oatpp::web::protocol::http::Status::CODE_401, "Unauthorized", {});

}