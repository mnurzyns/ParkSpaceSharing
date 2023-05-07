#pragma once

#include <oatpp/core/macro/codegen.hpp>

#include "AuthHandler.hh"
#include "TokenUtils.hh"
#include "dto/OfferDto.hh"
#include "dto/StatusDto.hh"
#include "service/OfferService.hh"
#include "service/PlaceService.hh"

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace server::controller {

class OfferController : public oatpp::web::server::api::ApiController
{
  private:
    server::service::OfferService offer_service_;
    server::service::PlaceService place_service_; // TODO: Make it a component?

  public:
    [[nodiscard]] explicit OfferController(
      std::shared_ptr<ObjectMapper> const& object_mapper,
      std::shared_ptr<TokenUtils> token_utils)
      : oatpp::web::server::api::ApiController(object_mapper)
    {
        setDefaultAuthorizationHandler(
          std::make_shared<AuthHandler>(std::move(token_utils)));
    }

    static std::shared_ptr<OfferController>
    createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                 object_mapper_component),
                 OATPP_COMPONENT(std::shared_ptr<TokenUtils>,
                                 token_utils_component))
    {
        return std::make_shared<OfferController>(object_mapper_component,
                                                 token_utils_component);
    }

    // Endpoints

    ENDPOINT_INFO(createOne)
    {
        info->summary = "Create one offer";
        info->tags.emplace_back("offer-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<oatpp::Object<dto::OfferDto>>(Status::CODE_200,
                                                        "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_400,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_409,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500,
                                                         "application/json");
    }

    ENDPOINT("POST",
             "offer",
             createOne,
             AUTHORIZATION(std::shared_ptr<TokenPayload>, auth_object),
             BODY_DTO(oatpp::Object<dto::OfferDto>, dto))
    {
        OATPP_ASSERT_HTTP(dto->place_id && dto->date_from && dto->date_to &&
                            dto->description && dto->price,
                          Status::CODE_400,
                          "Required parameter not provided")

        OATPP_ASSERT_HTTP(
          auth_object->user_role == Role::Admin ||
            auth_object->user_id ==
              place_service_.getOne(dto->place_id)->owner_id,
          Status::CODE_403,
          "Cannot create offer for another user's place as a regular user")

        return createDtoResponse(Status::CODE_200,
                                 offer_service_.createOne(dto));
    }

    ENDPOINT_INFO(getOne)
    {
        info->summary = "Get one offer";
        info->tags.emplace_back("offer-controller");

        info->addResponse<oatpp::Object<dto::OfferDto>>(Status::CODE_200,
                                                        "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500,
                                                         "application/json");
    }

    ENDPOINT("GET", "offer/{id}", getOne, PATH(UInt64, id))
    {
        return createDtoResponse(Status::CODE_200, offer_service_.getOne(id));
    }

    ENDPOINT_INFO(search)
    {
        info->summary = "Search offers";
        info->tags.emplace_back("offer-controller");

        info->queryParams["query"].required = false;
        info->queryParams["limit"].required = false;
        info->queryParams["offset"].required = false;

        info->addResponse<oatpp::Object<dto::OfferPageDto>>(Status::CODE_200,
                                                            "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404,
                                                         "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500,
                                                         "application/json");
    }

    ENDPOINT("GET",
             "offer",
             search,
             QUERY(String, query, "query", std::string{}),
             QUERY(UInt64, limit, "limit", uint64_t{ 20 }),
             QUERY(UInt64, offset, "offset", uint64_t{ 0 }))
    {
        return createDtoResponse(Status::CODE_200,
                                 offer_service_.search(query, limit, offset));
    }

    ENDPOINT_INFO(putOne)
    {
        info->summary = "Put one offer";
        info->tags.emplace_back("offer-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<oatpp::Object<dto::OfferDto>>(Status::CODE_200,
                                                        "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_400,
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

    ENDPOINT("PUT",
             "offer",
             putOne,
             AUTHORIZATION(std::shared_ptr<TokenPayload>, auth_object),
             BODY_DTO(oatpp::Object<dto::OfferDto>, dto))
    {
        OATPP_ASSERT_HTTP(dto->id && dto->place_id && dto->date_from &&
                            dto->date_to && dto->description && dto->price,
                          Status::CODE_400,
                          "Required parameter not provided")

        OATPP_ASSERT_HTTP(auth_object->user_role == Role::Admin ||
                            auth_object->user_id ==
                              place_service_.getOne(dto->place_id)->owner_id,
                          Status::CODE_403,
                          "Cannot create or modify offer for another user's "
                          "place as a regular user")

        return createDtoResponse(Status::CODE_200, offer_service_.putOne(dto));
    }

    ENDPOINT_INFO(patchOne)
    {
        info->summary = "Update one offer";
        info->tags.emplace_back("offer-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<oatpp::Object<dto::OfferDto>>(Status::CODE_200,
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
             "offer/{id}",
             patchOne,
             AUTHORIZATION(std::shared_ptr<TokenPayload>, auth_object),
             PATH(UInt64, id),
             BODY_DTO(oatpp::Object<dto::OfferDto>, dto))
    {
        OATPP_ASSERT_HTTP(
          auth_object->user_role == Role::Admin ||
            auth_object->user_id ==
              place_service_.getOne(offer_service_.getOne(id)->place_id)
                ->owner_id,
          Status::CODE_403,
          "Cannot modify offer for another user's place as a regular user")

        return createDtoResponse(Status::CODE_200,
                                 offer_service_.patchOne(id, dto));
    }

    ENDPOINT_INFO(deleteOne)
    {
        info->summary = "Delete one offer";
        info->tags.emplace_back("offer-controller");
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
             "offer/{id}",
             deleteOne,
             AUTHORIZATION(std::shared_ptr<TokenPayload>, auth_object),
             PATH(UInt64, id))
    {
        OATPP_ASSERT_HTTP(
          auth_object->user_role == Role::Admin ||
            place_service_.getOne(offer_service_.getOne(id)->place_id)
                ->owner_id == auth_object->user_id,
          Status::CODE_403,
          "Cannot delete offer for another user's place as a regular user")

        return createDtoResponse(Status::CODE_200,
                                 offer_service_.deleteOne(id));
    }
};

} // namespace server::controller

#include OATPP_CODEGEN_END(ApiController)
#include "TokenPayload.hh"
