#pragma once

#include <oatpp/core/macro/codegen.hpp>

#include "auth/AuthHandler.hh"
#include "auth/JWT.hh"
#include "dto/StatusDto.hh"
#include "dto/OfferDto.hh"
#include "service/PlaceService.hh"

namespace server::controller {

#include OATPP_CODEGEN_BEGIN(ApiController)

class PlaceController :
        public ::oatpp::web::server::api::ApiController {
private:
    server::service::PlaceService service_;

public:
    [[nodiscard]]
    explicit
    PlaceController(const std::shared_ptr<ObjectMapper> &objectMapper, const std::shared_ptr<auth::JWT> &jwtObject)
            : oatpp::web::server::api::ApiController(objectMapper) {
        setDefaultAuthorizationHandler(std::make_shared<auth::AuthHandler>(jwtObject));
    }

    static std::shared_ptr<PlaceController> createShared(
            OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper),
            OATPP_COMPONENT(std::shared_ptr<auth::JWT>, jwt_)
    ) {
        return std::make_shared<PlaceController>(objectMapper, jwt_);
    }

    // Endpoints

    ENDPOINT_INFO(createOne) {
        info->summary = "Create one place";
        info->tags.emplace_back("place-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_200, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_400, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
    }

    ENDPOINT("POST", "place", createOne,
             AUTHORIZATION(std::shared_ptr<auth::JWT::Payload>, authObject),
             BODY_DTO(oatpp::Object<dto::PlaceDto>, dto)) {
        OATPP_ASSERT_HTTP(
                dto->ownerId &&
                dto->address &&
                dto->latitude &&
                dto->longitude,
                Status::CODE_400,
                "Required parameter not provided"
        )
        dto->ownerId = authObject->userId;
        return createDtoResponse(Status::CODE_200, service_.createOne(dto));
    }

    ENDPOINT_INFO(getOne) {
        info->summary = "Get one place";
        info->tags.emplace_back("place-controller");

        info->addResponse<oatpp::Object<dto::PlaceDto>>(Status::CODE_200, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
    }

    ENDPOINT("GET", "place/{placeId}", getOne,
             PATH(UInt64, placeId)) {
        return createDtoResponse(Status::CODE_200, service_.getOne(placeId));
    }

    ENDPOINT_INFO(putOne) {
        info->summary = "Put one place";
        info->tags.emplace_back("place-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<oatpp::Object<dto::PlaceDto>>(Status::CODE_200, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_400, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
    }

    ENDPOINT("PUT", "place", putOne,
             AUTHORIZATION(std::shared_ptr<auth::JWT::Payload>, authObject),
             BODY_DTO(oatpp::Object<dto::PlaceDto>, dto)) {
        OATPP_ASSERT_HTTP(
                dto->id &&
                dto->ownerId &&
                dto->address &&
                dto->latitude &&
                dto->longitude,
                Status::CODE_400,
                "Required parameter not provided"
        )
        try {
            OATPP_ASSERT_HTTP(
                    authObject->role == 0 ||
                    (dto->ownerId == authObject->userId &&
                    service_.getOne(dto->id)->ownerId == authObject->userId),
                    Status::CODE_403,
                    "Cannot create or modify other user's place as a regular user"
            )
        } catch (oatpp::web::protocol::http::HttpError &error) {
            if (error.getInfo().status != Status::CODE_404) { throw; }
        }
        return createDtoResponse(Status::CODE_200, service_.putOne(dto));
    }

    ENDPOINT_INFO(patchOne) {
        info->summary = "Update one place";
        info->tags.emplace_back("place-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<oatpp::Object<dto::PlaceDto>>(Status::CODE_200, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
    }

    ENDPOINT("PATCH", "place/{placeId}", patchOne,
             AUTHORIZATION(std::shared_ptr<auth::JWT::Payload>, authObject),
             PATH(UInt64, placeId),
             BODY_DTO(oatpp::Object<dto::PlaceDto>, dto)) {
        OATPP_ASSERT_HTTP(
                authObject->role == 0 ||
                ((dto->ownerId == nullptr ||
                dto->ownerId == authObject->userId) &&
                service_.getOne(placeId)->ownerId == authObject->userId),
                Status::CODE_403,
                "Cannot modify other user's place as a regular user"
        )
        return createDtoResponse(Status::CODE_200, service_.patchOne(placeId, dto));
    }

    ENDPOINT_INFO(deleteOne) {
        info->summary = "Delete one place";
        info->tags.emplace_back("place-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_200, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
    }

    ENDPOINT("DELETE", "place/{placeId}", deleteOne,
             AUTHORIZATION(std::shared_ptr<auth::JWT::Payload>, authObject),
             PATH(UInt64, placeId)) {
        OATPP_ASSERT_HTTP(
                authObject->role == 0 ||
                service_.getOne(placeId)->ownerId == authObject->userId,
                Status::CODE_403,
                "Cannot delete other user's place as a regular user"
        )
        return createDtoResponse(Status::CODE_200, service_.deleteOne(placeId));
    }
};

#include OATPP_CODEGEN_END(ApiController)

} // namespace server::controller