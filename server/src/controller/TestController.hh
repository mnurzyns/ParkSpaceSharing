#pragma once

#include <oatpp/core/macro/codegen.hpp>

#include "auth/AuthHandler.hh"
#include "dto/StatusDto.hh"

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace server::controller {

class TestController : public oatpp::web::server::api::ApiController {

public:
    [[nodiscard]]
    explicit
    TestController(const std::shared_ptr<ObjectMapper>& objectMapper, const std::shared_ptr<auth::JWT>& jwtObject)
        : oatpp::web::server::api::ApiController(objectMapper) {
        setDefaultAuthorizationHandler(std::make_shared<auth::AuthHandler>(jwtObject));
    }

    static std::shared_ptr<TestController> create_shared(
        OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper),
        OATPP_COMPONENT(std::shared_ptr<auth::JWT>, jwtObject)
    ) {
        return std::make_shared<TestController>(objectMapper, jwtObject);
    }

    // Endpoints

    ENDPOINT_INFO(testPublic) {
        info->summary = "Public test endpoint";
        info->tags.emplace_back("test-controller");

        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_200, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "test/public", testPublic) {
        auto dto = dto::StatusDto::createShared();
        dto->code = Status::CODE_200.code;
        dto->status = "OK";
        dto->message = "Success";
        return createDtoResponse(Status::CODE_200, dto);
    }

    ENDPOINT_INFO(testPrivate) {
        info->summary = "Public test endpoint";
        info->tags.emplace_back("test-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_200, "application/json");
        //TODO: Make 401 status return a StatusDto
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_403, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "test/private", testPrivate,
             AUTHORIZATION(std::shared_ptr<auth::JWT::Payload>, authObject)) {
        OATPP_ASSERT_HTTP(authObject->isAdmin, Status::CODE_403, "Forbidden");
        auto dto = dto::StatusDto::createShared();
        dto->code = Status::CODE_200.code;
        dto->status = "OK";
        dto->message = "Success";
        return createDtoResponse(Status::CODE_200, dto);
    }
};

} // namespace server::controller

#include OATPP_CODEGEN_END(ApiController)
