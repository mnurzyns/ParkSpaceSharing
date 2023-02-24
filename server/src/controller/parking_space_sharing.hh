#pragma once

#include "dto/page.hh"
#include "dto/parking_space.hh"
#include "dto/status.hh"
#include "service/parking_space_sharing.hh"

#include <memory>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

namespace server::controller
{

#include OATPP_CODEGEN_BEGIN(ApiController)

class parking_space_sharing :
    public ::oatpp::web::server::api::ApiController
{
public:
    [[nodiscard]]
    explicit
    parking_space_sharing(OATPP_COMPONENT(::std::shared_ptr<ObjectMapper>, object_mapper)) :
        ::oatpp::web::server::api::ApiController{object_mapper} {}

    static
    ::std::shared_ptr<parking_space_sharing>
    create_shared(OATPP_COMPONENT(::std::shared_ptr<ObjectMapper>, object_mapper))
    {
        return ::std::make_shared<::server::controller::parking_space_sharing>(object_mapper);
    }

    ENDPOINT_INFO(get_users)
    {
        info->summary = "Get all users";

        info->addResponse<::oatpp::Object<::server::dto::user_all>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "users", get_users)
    {
        return createDtoResponse(Status::CODE_200, pss_service_.get_users());
    }

    ENDPOINT_INFO(get_user)
    {
        info->summary = "Get user by user.id";

        info->addResponse<::oatpp::Object<::server::dto::user>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status>>(Status::CODE_404, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "users/{user_id}", get_user, PATH(UInt32, user_id))
    {
        return createDtoResponse(Status::CODE_200, pss_service_.get_user(user_id));
    }

    ENDPOINT_INFO(create_user)
    {
        info->summary = "Create a user";

        info->addResponse<::oatpp::Object<::server::dto::user>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "users", create_user, BODY_DTO(Object<::server::dto::user>, user_dto))
    {
        return createDtoResponse(Status::CODE_200, pss_service_.create_user(user_dto));
    }

    ENDPOINT_INFO(get_parking_spaces)
    {
        info->summary = "Get all parking spaces";

        info->addResponse<::oatpp::Object<::server::dto::parking_space_all>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status>>(Status::CODE_404, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "parking_spaces", get_parking_spaces)
    {
        return createDtoResponse(Status::CODE_200, pss_service_.get_parking_spaces());
    }

private:
    ::server::service::parking_space_sharing pss_service_;
};

#include OATPP_CODEGEN_END(ApiController)

} // namespace server::controller

