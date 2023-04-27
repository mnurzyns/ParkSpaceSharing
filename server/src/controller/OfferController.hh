#pragma once

#include <oatpp/core/macro/codegen.hpp>

#include "auth/AuthHandler.hh"
#include "auth/JWT.hh"
#include "dto/StatusDto.hh"
#include "dto/OfferDto.hh"
#include "service/OfferService.hh"
#include "service/PlaceService.hh"

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace server::controller {

    class OfferController :
            public oatpp::web::server::api::ApiController {
    private:
        server::service::OfferService offerService_;
        server::service::PlaceService placeService_; // TODO: Make it a component?

    public:
        [[nodiscard]]
        explicit
        OfferController(
                std::shared_ptr<ObjectMapper> const &objectMapper,
                std::shared_ptr<auth::JWT> const &jwtObject
        )
                : oatpp::web::server::api::ApiController(objectMapper) {
            setDefaultAuthorizationHandler(std::make_shared<auth::AuthHandler>(jwtObject));
        }

        static std::shared_ptr<OfferController>
        createShared(
                OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper),
                OATPP_COMPONENT(std::shared_ptr<auth::JWT>, jwt_)
        ) {
            return std::make_shared<OfferController>(objectMapper, jwt_);
        }

        // Endpoints

        ENDPOINT_INFO(createOne) {
            info->summary = "Create one offer";
            info->tags.emplace_back("offer-controller");
            info->addSecurityRequirement("JWT Bearer Auth", {});

            info->addResponse<oatpp::Object<dto::OfferDto>>(Status::CODE_200, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_400, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_409, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
        }

        ENDPOINT("POST", "offer", createOne,
                 AUTHORIZATION(std::shared_ptr<auth::JWT::Payload>, authObject),
                 BODY_DTO(oatpp::Object<dto::OfferDto>, dto)) {
            OATPP_ASSERT_HTTP(
                    dto->placeId &&
                    dto->dateFrom &&
                    dto->dateTo &&
                    dto->description &&
                    dto->price,
                    Status::CODE_400,
                    "Required parameter not provided"
            )
            OATPP_ASSERT_HTTP(
                    authObject->role == 0 ||
                    authObject->userId == placeService_.getOne(dto->placeId)->ownerId,
                    Status::CODE_403,
                    "Cannot create offer for another user's place as a regular user"
            )
            return createDtoResponse(Status::CODE_200, offerService_.createOne(dto));
        }

        ENDPOINT_INFO(getOne) {
            info->summary = "Get one offer";
            info->tags.emplace_back("offer-controller");

            info->addResponse<oatpp::Object<dto::OfferDto>>(Status::CODE_200, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
        }

        ENDPOINT("GET", "offer/{id}", getOne,
                 PATH(UInt64, id)) {
            return createDtoResponse(Status::CODE_200, offerService_.getOne(id));
        }

        ENDPOINT_INFO(search) {
            info->summary = "Search offers";
            info->tags.emplace_back("offer-controller");

            info->queryParams["query"].required = false;
            info->queryParams["limit"].required = false;
            info->queryParams["offset"].required = false;

            info->addResponse<oatpp::Object<dto::OfferPageDto>>(Status::CODE_200, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
        }

        ENDPOINT("GET", "offer", search,
                 QUERY(String, query, "query", std::string{}),
                 QUERY(UInt64, limit, "limit", 20U),
                 QUERY(UInt64, offset, "offset", 0UL)) {
            return createDtoResponse(Status::CODE_200, offerService_.search(query, limit, offset));
        }

        ENDPOINT_INFO(putOne) {
            info->summary = "Put one offer";
            info->tags.emplace_back("offer-controller");
            info->addSecurityRequirement("JWT Bearer Auth", {});

            info->addResponse<oatpp::Object<dto::OfferDto>>(Status::CODE_200, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_400, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
        }

        ENDPOINT("PUT", "offer", putOne,
                 AUTHORIZATION(std::shared_ptr<auth::JWT::Payload>, authObject),
                 BODY_DTO(oatpp::Object<dto::OfferDto>, dto)) {
            OATPP_ASSERT_HTTP(
                    dto->id &&
                    dto->placeId &&
                    dto->dateFrom &&
                    dto->dateTo &&
                    dto->description &&
                    dto->price,
                    Status::CODE_400,
                    "Required parameter not provided"
            )
            OATPP_ASSERT_HTTP(
                    authObject->role == 0 ||
                    authObject->userId == placeService_.getOne(dto->placeId)->ownerId,
                    Status::CODE_403,
                    "Cannot create or modify offer for another user's place as a regular user"
            )
            return createDtoResponse(Status::CODE_200, offerService_.putOne(dto));
        }

        ENDPOINT_INFO(patchOne) {
            info->summary = "Update one offer";
            info->tags.emplace_back("offer-controller");
            info->addSecurityRequirement("JWT Bearer Auth", {});

            info->addResponse<oatpp::Object<dto::OfferDto>>(Status::CODE_200, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
        }

        ENDPOINT("PATCH", "offer/{id}", patchOne,
                 AUTHORIZATION(std::shared_ptr<auth::JWT::Payload>, authObject),
                 PATH(UInt64, id),
                 BODY_DTO(oatpp::Object<dto::OfferDto>, dto)) {
            OATPP_ASSERT_HTTP(
                    authObject->role == 0 ||
                    authObject->userId == placeService_.getOne(dto->placeId)->ownerId,
                    Status::CODE_403,
                    "Cannot modify offer for another user's place as a regular user"
            )
            return createDtoResponse(Status::CODE_200, offerService_.patchOne(id, dto));
        }

        ENDPOINT_INFO(deleteOne) {
            info->summary = "Delete one offer";
            info->tags.emplace_back("offer-controller");
            info->addSecurityRequirement("JWT Bearer Auth", {});

            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_200, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
        }

        ENDPOINT("DELETE", "offer/{id}", deleteOne,
                 AUTHORIZATION(std::shared_ptr<auth::JWT::Payload>, authObject),
                 PATH(UInt64, id)) {
            OATPP_ASSERT_HTTP(
                    authObject->role == 0 ||
                    placeService_.getOne(offerService_.getOne(id)->placeId)->ownerId == authObject->userId,
                    Status::CODE_403,
                    "Cannot delete offer for another user's place as a regular user"
            )
            return createDtoResponse(Status::CODE_200, offerService_.deleteOne(id));
        }
    };

} // namespace server::controller

#include OATPP_CODEGEN_END(ApiController)
