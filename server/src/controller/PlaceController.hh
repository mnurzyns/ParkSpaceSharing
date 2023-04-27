#pragma once

#include <oatpp/core/macro/codegen.hpp>

#include "auth/AuthHandler.hh"
#include "auth/JWT.hh"
#include "dto/StatusDto.hh"
#include "dto/OfferDto.hh"
#include "service/PlaceService.hh"

using HttpError = oatpp::web::protocol::http::HttpError;

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace server::controller {

    class PlaceController :
            public oatpp::web::server::api::ApiController {
    private:
        server::service::PlaceService service_;

    public:
        [[nodiscard]]
        explicit
        PlaceController(
                std::shared_ptr<ObjectMapper> const &objectMapper,
                std::shared_ptr<auth::JWT> const &jwtObject
        )
                : oatpp::web::server::api::ApiController(objectMapper) {
            setDefaultAuthorizationHandler(std::make_shared<auth::AuthHandler>(jwtObject));
        }

        static std::shared_ptr<PlaceController>
        createShared(
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

            info->addResponse<oatpp::Object<dto::PlaceDto>>(Status::CODE_200, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_400, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_409, "application/json");
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
            OATPP_ASSERT_HTTP(
                    authObject->role == 0 ||
                    authObject->userId == dto->ownerId,
                    Status::CODE_403,
                    "Cannot create place for another user as a regular user"
            )
            return createDtoResponse(Status::CODE_200, service_.createOne(dto));
        }

        ENDPOINT_INFO(getOne) {
            info->summary = "Get one place";
            info->tags.emplace_back("place-controller");

            info->addResponse<oatpp::Object<dto::PlaceDto>>(Status::CODE_200, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
        }

        ENDPOINT("GET", "place/{id}", getOne,
                 PATH(UInt64, id)) {
            return createDtoResponse(Status::CODE_200, service_.getOne(id));
        }

        ENDPOINT_INFO(search) {
            info->summary = "Search places";
            info->tags.emplace_back("place-controller");

            info->queryParams["query"].required = false;
            info->queryParams["limit"].required = false;
            info->queryParams["offset"].required = false;

            info->addResponse<oatpp::Object<dto::PlacePageDto>>(Status::CODE_200, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
        }

        ENDPOINT("GET", "place", search,
                 QUERY(String, query, "query", std::string{}),
                 QUERY(UInt64, limit, "limit", 20U),
                 QUERY(UInt64, offset, "offset", uint64_t{0})) {
            return createDtoResponse(Status::CODE_200, service_.search(query, limit, offset));
        }

        ENDPOINT_INFO(putOne) {
            info->summary = "Put one place";
            info->tags.emplace_back("place-controller");
            info->addSecurityRequirement("JWT Bearer Auth", {});

            info->addResponse<oatpp::Object<dto::PlaceDto>>(Status::CODE_200, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_400, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403, "application/json");
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
                        (authObject->userId == service_.getOne(dto->id)->ownerId &&
                         (dto->ownerId == nullptr || dto->ownerId == authObject->userId)),
                        Status::CODE_403,
                        "Cannot create or modify other user's places as a regular user"
                )
            } catch (HttpError &error) {
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
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
        }

        ENDPOINT("PATCH", "place/{id}", patchOne,
                 AUTHORIZATION(std::shared_ptr<auth::JWT::Payload>, authObject),
                 PATH(UInt64, id),
                 BODY_DTO(oatpp::Object<dto::PlaceDto>, dto)) {
            OATPP_ASSERT_HTTP(
                    authObject->role == 0 ||
                    (authObject->userId == service_.getOne(id)->ownerId &&
                     (dto->ownerId == nullptr || dto->ownerId == authObject->userId)),
                    Status::CODE_403,
                    "Cannot modify other user's places as a regular user"
            )
            return createDtoResponse(Status::CODE_200, service_.patchOne(id, dto));
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

        ENDPOINT("DELETE", "place/{id}", deleteOne,
                 AUTHORIZATION(std::shared_ptr<auth::JWT::Payload>, authObject),
                 PATH(UInt64, id)) {
            OATPP_ASSERT_HTTP(
                    authObject->role == 0 ||
                    service_.getOne(id)->ownerId == authObject->userId,
                    Status::CODE_403,
                    "Cannot delete other user's places as a regular user"
            )
            return createDtoResponse(Status::CODE_200, service_.deleteOne(id));
        }
    };

} // namespace server::controller

#include OATPP_CODEGEN_END(ApiController)
