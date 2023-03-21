#pragma once

#include "dto/page_dto.hh"
#include "dto/offer_dto.hh"
#include "dto/status_dto.hh"
#include "service/user_service.hh"

#include <memory>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

namespace server::controller
{

#include OATPP_CODEGEN_BEGIN(ApiController)

class user_controller :
    public ::oatpp::web::server::api::ApiController
{
public:
    [[nodiscard]]
    explicit
    user_controller(OATPP_COMPONENT(::std::shared_ptr<ObjectMapper>, object_mapper)) :
        ::oatpp::web::server::api::ApiController{object_mapper} {}

    static
    ::std::shared_ptr<user_controller>
    create_shared(OATPP_COMPONENT(::std::shared_ptr<ObjectMapper>, object_mapper))
    {
        return ::std::make_shared<::server::controller::user_controller>(object_mapper);
    }

    ENDPOINT_INFO(get_users)
    {
        info->summary = "Get users (functon only for testing purposes)";
        info->tags.emplace_back("user-controller");

        info->addResponse<::oatpp::Object<::server::dto::user_page_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "users", get_users)
    {
        return createDtoResponse(Status::CODE_200, service_.get_users());
    }

    ENDPOINT_INFO(get_myUser)
    {
        info->summary = "Get user by token";
        info->tags.emplace_back("user-controller");

        info->addResponse<::oatpp::Object<::server::dto::user_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_404, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "user/{user_token}", get_myUser, PATH(oatpp::String, user_token))
    {
        return createDtoResponse(Status::CODE_200, service_.get_myUser(user_token));
    }

    ENDPOINT_INFO(create_user)
    {
        info->summary = "Create a user (functon only for testing purposes)";
        info->tags.emplace_back("user-controller");

        info->addResponse<::oatpp::Object<::server::dto::user_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "users", create_user, BODY_DTO(Object<::server::dto::user_dto>, user_dto))
    {
        return createDtoResponse(Status::CODE_200, service_.create_user(user_dto));
    }

private:
    ::server::service::user_service service_;
};

#include OATPP_CODEGEN_END(ApiController)

} // namespace server::controller

