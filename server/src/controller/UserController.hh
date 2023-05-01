#pragma once

#include <oatpp/core/macro/codegen.hpp>

#include "AuthHandler.hh"
#include "dto/StatusDto.hh"
#include "dto/UserDto.hh"
#include "service/UserService.hh"

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace server::controller {

class UserController : public oatpp::web::server::api::ApiController
{
  private:
    server::service::UserService service_;

  public:
    [[nodiscard]] explicit UserController(
      std::shared_ptr<ObjectMapper> const& object_mapper,
      std::shared_ptr<TokenUtils> token_utils)
      : oatpp::web::server::api::ApiController(object_mapper)
    {
        setDefaultAuthorizationHandler(
          std::make_shared<AuthHandler>(std::move(token_utils)));
    }

    static std::shared_ptr<UserController>
    createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                 object_mapper_component),
                 OATPP_COMPONENT(std::shared_ptr<TokenUtils>,
                                 token_utils_component))
    {
        return std::make_shared<UserController>(object_mapper_component,
                                                token_utils_component);
    }

    // Endpoints

    ENDPOINT_INFO(createOne)
    {
        info->summary = "Create one user";
        info->tags.emplace_back("user-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<oatpp::Object<dto::UserDto>>(Status::CODE_200,
                                                       "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_400,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_409,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500,
                                                         "application/json");
    }

    ENDPOINT("POST",
             "user",
             createOne,
             AUTHORIZATION(std::shared_ptr<TokenPayload>, auth_object),
             BODY_DTO(oatpp::Object<dto::UserDto>, dto))
    {
        OATPP_ASSERT_HTTP(dto->email && dto->password && dto->username &&
                            dto->role,
                          Status::CODE_400,
                          "Required parameter not provided")

        OATPP_ASSERT_HTTP(auth_object->user_role == Role::Admin,
                          Status::CODE_403,
                          "Cannot create user as a regular user")

        return createDtoResponse(Status::CODE_200, service_.createOne(dto));
    }

    ENDPOINT_INFO(getOne)
    {
        info->summary = "Get one user";
        info->tags.emplace_back("user-controller");

        info->addResponse<oatpp::Object<dto::UserDto>>(Status::CODE_200,
                                                       "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500,
                                                         "application/json");
    }

    ENDPOINT("GET", "user/{id}", getOne, PATH(UInt64, id))
    {
        return createDtoResponse(Status::CODE_200, service_.getOne(id));
    }

    ENDPOINT_INFO(search)
    {
        info->summary = "Search users";
        info->tags.emplace_back("user-controller");

        info->queryParams["query"].required = false;
        info->queryParams["limit"].required = false;
        info->queryParams["offset"].required = false;

        info->addResponse<oatpp::Object<dto::UserPageDto>>(Status::CODE_200,
                                                           "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500,
                                                         "application/json");
    }

    ENDPOINT("GET",
             "user",
             search,
             QUERY(String, query, "query", std::string{}),
             QUERY(UInt64, limit, "limit", uint64_t{ 20 }),
             QUERY(UInt64, offset, "offset", uint64_t{ 0 }))
    {
        return createDtoResponse(Status::CODE_200,
                                 service_.search(query, limit, offset));
    }

    ENDPOINT_INFO(putOne)
    {
        info->summary = "Put one user";
        info->tags.emplace_back("user-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<oatpp::Object<dto::UserDto>>(Status::CODE_200,
                                                       "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_400,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500,
                                                         "application/json");
    }

    ENDPOINT("PUT",
             "user",
             putOne,
             AUTHORIZATION(std::shared_ptr<TokenPayload>, auth_object),
             BODY_DTO(oatpp::Object<dto::UserDto>, dto))
    {
        OATPP_ASSERT_HTTP(dto->id && dto->email && dto->password &&
                            dto->username,
                          Status::CODE_400,
                          "Required parameter not provided")

        OATPP_ASSERT_HTTP(
          auth_object->user_role == Role::Admin,
          Status::CODE_403,
          "Cannot create or modify other user as a regular user")

        return createDtoResponse(Status::CODE_200, service_.putOne(dto));
    }

    ENDPOINT_INFO(patchOne)
    {
        info->summary = "Update one user";
        info->tags.emplace_back("user-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<oatpp::Object<dto::UserDto>>(Status::CODE_200,
                                                       "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500,
                                                         "application/json");
    }

    ENDPOINT("PATCH",
             "user/{id}",
             patchOne,
             AUTHORIZATION(std::shared_ptr<TokenPayload>, auth_object),
             PATH(UInt64, id),
             BODY_DTO(oatpp::Object<dto::UserDto>, dto))
    {
        auto const existing = service_.getOne(id);
        OATPP_ASSERT_HTTP(auth_object->user_role == Role::Admin ||
                            auth_object->user_id == id,
                          Status::CODE_403,
                          "Cannot modify other user as a regular user")

        OATPP_ASSERT_HTTP(auth_object->user_role == Role::Admin ||
                            dto->role == existing->role,
                          Status::CODE_403,
                          "Cannot modify role as a regular user")

        return createDtoResponse(Status::CODE_200, service_.patchOne(id, dto));
    }

    ENDPOINT_INFO(deleteOne)
    {
        info->summary = "Delete one user";
        info->tags.emplace_back("user-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_200,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500,
                                                         "application/json");
    }

    ENDPOINT("DELETE",
             "user/{id}",
             deleteOne,
             AUTHORIZATION(std::shared_ptr<TokenPayload>, auth_object),
             PATH(UInt64, id))
    {
        OATPP_ASSERT_HTTP(auth_object->user_role == Role::Admin ||
                            service_.getOne(id)->id == auth_object->user_id,
                          Status::CODE_403,
                          "Cannot delete another user as a regular user")

        return createDtoResponse(Status::CODE_200, service_.deleteOne(id));
    }
};

} // namespace server::controller

#include OATPP_CODEGEN_END(ApiController)
#include "TokenPayload.hh"
#include "TokenUtils.hh"
