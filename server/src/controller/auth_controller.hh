#pragma once

#include "dto/page_dto.hh"
#include "dto/status_dto.hh"
#include "dto/signIn_dto.hh"
#include "dto/signUp_dto.hh"
#include "dto/auth_dto.hh"
#include "service/auth_service.hh"

#include <memory>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

namespace server::controller
{

#include OATPP_CODEGEN_BEGIN(ApiController)

class auth_controller :
    public ::oatpp::web::server::api::ApiController
{
public:
    [[nodiscard]]
    explicit
    auth_controller(OATPP_COMPONENT(::std::shared_ptr<ObjectMapper>, object_mapper)) :
        ::oatpp::web::server::api::ApiController{object_mapper} {}

    static
    ::std::shared_ptr<auth_controller>
    create_shared(OATPP_COMPONENT(::std::shared_ptr<ObjectMapper>, object_mapper))
    {
        return ::std::make_shared<::server::controller::auth_controller>(object_mapper);
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

