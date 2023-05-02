#pragma once

#include <oatpp/web/server/api/ApiController.hpp>

#include "TokenPayload.hh"
#include "dto/AuthDto.hh"
#include "dto/SignInDto.hh"
#include "dto/SignUpDto.hh"
#include "dto/StatusDto.hh"
#include "service/AuthService.hh"

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace server::controller {

using dto::AuthDto, dto::SignInDto, dto::SignUpDto, dto::StatusDto;

class AuthController : public oatpp::web::server::api::ApiController
{

  private:
    server::service::AuthService service_;

  public:
    [[nodiscard]] explicit AuthController(
      std::shared_ptr<ObjectMapper> const& object_mapper)
      : oatpp::web::server::api::ApiController(object_mapper)
    {
    }

    static std::shared_ptr<AuthController>
    createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                 object_mapper_component))
    {
        return std::make_shared<AuthController>(object_mapper_component);
    }

    ENDPOINT_INFO(signUp)
    {
        info->summary = "Sign up";
        info->tags.emplace_back("auth-controller");

        info->addConsumes<Object<SignUpDto>>("application/json");

        info->addResponse<Object<StatusDto>>(Status::CODE_201,
                                             "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_400,
                                             "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_409,
                                             "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_500,
                                             "application/json");
    }

    ENDPOINT("POST", "signup", signUp, BODY_DTO(Object<SignUpDto>, dto))
    {
        OATPP_ASSERT_HTTP(dto->email && dto->password && dto->username,
                          Status::CODE_400,
                          "Required parameter not provided")

        return createDtoResponse(Status::CODE_201, service_.signUp(dto));
    }

    ENDPOINT_INFO(signIn)
    {
        info->summary = "Sign in";
        info->tags.emplace_back("auth-controller");

        info->addConsumes<Object<SignInDto>>("application/json");

        info->addResponse<Object<AuthDto>>(Status::CODE_200,
                                           "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_400,
                                             "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_401,
                                             "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_404,
                                             "application/json");
        info->addResponse<Object<StatusDto>>(Status::CODE_500,
                                             "application/json");
    }

    ENDPOINT("POST", "signin", signIn, BODY_DTO(Object<SignInDto>, dto))
    {
        OATPP_ASSERT_HTTP(dto->login && dto->password,
                          Status::CODE_400,
                          "Required parameter not provided")

        return createDtoResponse(Status::CODE_200, service_.signIn(dto));
    }
};

} // namespace server::controller

#include OATPP_CODEGEN_END(ApiController)
