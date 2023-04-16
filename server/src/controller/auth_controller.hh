#pragma once

#include "service/auth_service.hh"

#include <memory>
#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>


namespace server::controller
{

#include OATPP_CODEGEN_BEGIN(ApiController)

class auth_controller :
    public ::oatpp::web::server::api::ApiController
{
public:
    [[nodiscard]]
    explicit
    auth_controller(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper) {}

    static std::shared_ptr<auth_controller> create_shared(
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)
    ) {
        return std::make_shared<auth_controller>(objectMapper);
    }
    
    ENDPOINT_INFO(signUp) {
        info->summary = "Sign up";
        info->tags.emplace_back("auth-controller");

        info->addConsumes<Object<::server::dto::signUp_dto>> ("application/json");

        info->addResponse<Object<::server::dto::auth_dto>> (Status::CODE_200, "application/json");
        info->addResponse<Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "users/signup", signUp, BODY_DTO(Object<::server::dto::signUp_dto>, dto))
    {
        return createDtoResponse(Status::CODE_200, service_.signUp(dto));
    }


    ENDPOINT_INFO(signIn) {
        info->summary = "Sign in";
        info->tags.emplace_back("auth-controller");

        info->addConsumes<Object<::server::dto::signIn_dto>> ("application/json");

        info->addResponse<Object<::server::dto::auth_dto>> (Status::CODE_200, "application/json");
        info->addResponse<Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "users/signin", signIn, BODY_DTO(Object<::server::dto::signIn_dto>, dto))
    {
        return createDtoResponse(Status::CODE_200, service_.signIn(dto));
    }

private:
    ::server::service::auth_service service_;
};

#include OATPP_CODEGEN_END(ApiController)

} // namespace server::controller