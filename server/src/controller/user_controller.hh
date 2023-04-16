#pragma once

#include "auth/auth_handler.hpp"
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
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>

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
        ::oatpp::web::server::api::ApiController{object_mapper} {
            setDefaultAuthorizationHandler(std::make_shared<auth_handler>("JWT Bearer Auth"));
        }

    static
    ::std::shared_ptr<user_controller>
    create_shared(OATPP_COMPONENT(::std::shared_ptr<ObjectMapper>, object_mapper))
    {
        return ::std::make_shared<::server::controller::user_controller>(object_mapper);
    }

    ENDPOINT_INFO(get_users)
    {
        info->summary = "Get users (for admin use)";
        info->tags.emplace_back("user_controller");

        info->addResponse<::oatpp::Object<::server::dto::user_page_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "users", get_users,
             AUTHORIZATION(std::shared_ptr<JWT::payload>, authObject))
    {
        OATPP_ASSERT_HTTP(authObject->isAdmin, Status::CODE_401, "Unauthorized");
        return createDtoResponse(Status::CODE_200, service_.get_users());
    }

    ENDPOINT_INFO(get_user_byId)
    {
        info->summary = "Get user by id (for admin use)";
        info->tags.emplace_back("user_controller");

        info->addResponse<::oatpp::Object<::server::dto::user_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_404, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "users/{user_id}", get_user_byId, PATH(oatpp::UInt32, user_id),
             AUTHORIZATION(std::shared_ptr<JWT::payload>, authObject))
    {
        OATPP_ASSERT_HTTP(authObject->isAdmin, Status::CODE_401, "Unauthorized");
        return createDtoResponse(Status::CODE_200, service_.get_user_byId(user_id));
    }

    ENDPOINT_INFO(create_user)
    {
        info->summary = "Create a user (for admin use)";
        info->tags.emplace_back("user_controller");

        info->addResponse<::oatpp::Object<::server::dto::user_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "users", create_user, BODY_DTO(Object<::server::dto::user_dto>, user_dto),
             AUTHORIZATION(std::shared_ptr<JWT::payload>, authObject))
    {
        OATPP_ASSERT_HTTP(authObject->isAdmin, Status::CODE_401, "Unauthorized");
        return createDtoResponse(Status::CODE_200, service_.create_user(user_dto));
    }

    ENDPOINT_INFO(delete_user) {
        info->summary = "Delete User (for admin use)";
        info->tags.emplace_back("user_controller");

        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_200, "application/json");

    }
    ENDPOINT("DELETE", "users/{user_id}", delete_user, PATH(oatpp::UInt32, userId))
    {
        return createDtoResponse(Status::CODE_200, service_.delete_user(userId));
    }


    

private:
    ::server::service::user_service service_;
};

#include OATPP_CODEGEN_END(ApiController)

} // namespace server::controller

