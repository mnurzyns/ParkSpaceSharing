#pragma once

#include <oatpp/core/macro/codegen.hpp>

#include "AuthHandler.hh"
#include "JWT.hh"
#include "dto/OfferDto.hh"
#include "dto/StatusDto.hh"
#include "service/PlaceService.hh"

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace server::controller {

using HttpError = oatpp::web::protocol::http::HttpError;

class PlaceController : public oatpp::web::server::api::ApiController
{
  private:
    server::service::PlaceService service_;

  public:
    [[nodiscard]] explicit PlaceController(
      std::shared_ptr<ObjectMapper> const& object_mapper,
      std::shared_ptr<JWT> const& jwt_object)
      : oatpp::web::server::api::ApiController(object_mapper)
    {
        setDefaultAuthorizationHandler(
          std::make_shared<AuthHandler>(jwt_object));
    }

    static std::shared_ptr<PlaceController>
    createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                 object_mapper_component),
                 OATPP_COMPONENT(std::shared_ptr<JWT>, jwt_object_component))
    {
        return std::make_shared<PlaceController>(object_mapper_component,
                                                 jwt_object_component);
    }

    // Endpoints

    ENDPOINT_INFO(createOne)
    {
        info->summary = "Create one place";
        info->tags.emplace_back("place-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<Object<dto::PlaceDto>>(Status::CODE_200,
                                                 "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_400,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_401,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_403,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_409,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_500,
                                                  "application/json");
    }

    ENDPOINT("POST",
             "place",
             createOne,
             AUTHORIZATION(std::shared_ptr<JWT::Payload>, auth_object),
             BODY_DTO(Object<dto::PlaceDto>, dto))
    {
        OATPP_ASSERT_HTTP(dto->owner_id && dto->address && dto->latitude &&
                            dto->longitude,
                          Status::CODE_400,
                          "Required parameter not provided")

        OATPP_ASSERT_HTTP(
          auth_object->role == 0 || auth_object->user_id == dto->owner_id,
          Status::CODE_403,
          "Cannot create place of another user as a regular user")

        return createDtoResponse(Status::CODE_200, service_.createOne(dto));
    }

    ENDPOINT_INFO(getOne)
    {
        info->summary = "Get one place";
        info->tags.emplace_back("place-controller");

        info->addResponse<Object<dto::PlaceDto>>(Status::CODE_200,
                                                 "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_404,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_500,
                                                  "application/json");
    }

    ENDPOINT("GET", "place/{id}", getOne, PATH(UInt64, id))
    {
        return createDtoResponse(Status::CODE_200, service_.getOne(id));
    }

    ENDPOINT_INFO(search)
    {
        info->summary = "Search places";
        info->tags.emplace_back("place-controller");

        info->queryParams["query"].required = false;
        info->queryParams["limit"].required = false;
        info->queryParams["offset"].required = false;

        info->addResponse<Object<dto::PlacePageDto>>(Status::CODE_200,
                                                     "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_404,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_500,
                                                  "application/json");
    }

    ENDPOINT("GET",
             "place",
             search,
             QUERY(String, query, "query", std::string{}),
             QUERY(UInt64, limit, "limit", 20U),
             QUERY(UInt64, offset, "offset", 0UL))
    {
        return createDtoResponse(Status::CODE_200,
                                 service_.search(query, limit, offset));
    }

    ENDPOINT_INFO(putOne)
    {
        info->summary = "Put one place";
        info->tags.emplace_back("place-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<Object<dto::PlaceDto>>(Status::CODE_200,
                                                 "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_400,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_401,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_403,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_500,
                                                  "application/json");
    }

    ENDPOINT("PUT",
             "place",
             putOne,
             AUTHORIZATION(std::shared_ptr<JWT::Payload>, auth_object),
             BODY_DTO(Object<dto::PlaceDto>, dto))
    {
        OATPP_ASSERT_HTTP(dto->id && dto->owner_id && dto->address &&
                            dto->latitude && dto->longitude,
                          Status::CODE_400,
                          "Required parameter not provided")

        try {
            OATPP_ASSERT_HTTP(
              auth_object->role == 0 ||
                (((dto->owner_id == nullptr &&
                   (dto->owner_id = auth_object->user_id)) && // NOLINT
                  auth_object->user_id == service_.getOne(dto->id)->owner_id) ||
                 dto->owner_id == auth_object->user_id),
              Status::CODE_403,
              "Cannot create or modify place of another user as a regular user")
        } catch (HttpError& error) {
            if (error.getInfo().status != Status::CODE_404) {
                throw;
            }
        }

        return createDtoResponse(Status::CODE_200, service_.putOne(dto));
    }

    ENDPOINT_INFO(patchOne)
    {
        info->summary = "Update one place";
        info->tags.emplace_back("place-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<Object<dto::PlaceDto>>(Status::CODE_200,
                                                 "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_401,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_403,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_404,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_500,
                                                  "application/json");
    }

    ENDPOINT("PATCH",
             "place/{id}",
             patchOne,
             AUTHORIZATION(std::shared_ptr<JWT::Payload>, auth_object),
             PATH(UInt64, id),
             BODY_DTO(Object<dto::PlaceDto>, dto))
    {
        OATPP_ASSERT_HTTP(
          auth_object->role == 0 ||
            (((dto->owner_id == nullptr &&
               (dto->owner_id = auth_object->user_id)) || // NOLINT
              dto->owner_id == auth_object->user_id) &&
             auth_object->user_id == service_.getOne(id)->owner_id),
          Status::CODE_403,
          "Cannot modify other place of another user as a regular user")

        return createDtoResponse(Status::CODE_200, service_.patchOne(id, dto));
    }

    ENDPOINT_INFO(deleteOne)
    {
        info->summary = "Delete one place";
        info->tags.emplace_back("place-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<Object<dto::StatusDto>>(Status::CODE_200,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_401,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_403,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_404,
                                                  "application/json");
        info->addResponse<Object<dto::StatusDto>>(Status::CODE_500,
                                                  "application/json");
    }

    ENDPOINT("DELETE",
             "place/{id}",
             deleteOne,
             AUTHORIZATION(std::shared_ptr<JWT::Payload>, auth_object),
             PATH(UInt64, id))
    {
        OATPP_ASSERT_HTTP(
          auth_object->role == 0 ||
            service_.getOne(id)->owner_id == auth_object->user_id,
          Status::CODE_403,
          "Cannot delete place of another user as a regular user")

        return createDtoResponse(Status::CODE_200, service_.deleteOne(id));
    }
};

} // namespace server::controller

#include OATPP_CODEGEN_END(ApiController)
#include "JWT.hh"
