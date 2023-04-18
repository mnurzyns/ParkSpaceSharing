#pragma once

#include <oatpp/web/server/api/ApiController.hpp>

#include "dto/AuthDto.hh"
#include "dto/SignInDto.hh"
#include "dto/SignUpDto.hh"
#include "dto/StatusDto.hh"
#include "service/AuthService.hh"

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace server::controller {

class AuthController : public oatpp::web::server::api::ApiController {

private:
  server::service::AuthService service_;

public:
    [[nodiscard]]
    explicit AuthController(const std::shared_ptr<ObjectMapper>& objectMapper)
        : oatpp::web::server::api::ApiController(objectMapper) {}

    static std::shared_ptr<AuthController> create_shared(
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)
    ) {
        return std::make_shared<AuthController>(objectMapper);
    }

    ENDPOINT_INFO(signUp) {
      info->summary = "Sign up";
      info->tags.emplace_back("auth-controller");

      info->addConsumes<Object<dto::SignUpDto>> ("application/json");

      info->addResponse<Object<dto::AuthDto>> (Status::CODE_200, "application/json");
      info->addResponse<Object<dto::StatusDto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "signup", signUp, BODY_DTO(Object<dto::SignUpDto>, dto))
    {
      return createDtoResponse(Status::CODE_200, service_.signUp(dto));
    }


    ENDPOINT_INFO(signIn) {
      info->summary = "Sign in";
      info->tags.emplace_back("auth-controller");

      info->addConsumes<Object<dto::SignInDto>> ("application/json");

      info->addResponse<Object<dto::AuthDto>> (Status::CODE_200, "application/json");
      info->addResponse<Object<dto::StatusDto>>(Status::CODE_401, "application/json");
      info->addResponse<Object<dto::StatusDto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "signin", signIn, BODY_DTO(Object<dto::SignInDto>, dto))
    {
      return createDtoResponse(Status::CODE_200, service_.signIn(dto));
    }


};

}  // namespace server::controller

#include OATPP_CODEGEN_END(ApiController)
