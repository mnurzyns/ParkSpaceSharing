#pragma once

#include <oatpp/core/macro/codegen.hpp>

#include "auth/AuthHandler.hh"
#include "auth/JWT.hh"
#include "dto/StatusDto.hh"
#include "dto/OfferDto.hh"
#include "service/OfferService.hh"


namespace server::controller {

#include OATPP_CODEGEN_BEGIN(ApiController)

class OfferController :
        public ::oatpp::web::server::api::ApiController {
private:
    service::OfferService service_;
public:
    [[nodiscard]]
    explicit
    OfferController(const std::shared_ptr<ObjectMapper> &objectMapper, const std::shared_ptr<auth::JWT> &jwtObject)
            : oatpp::web::server::api::ApiController(objectMapper) {
        setDefaultAuthorizationHandler(std::make_shared<auth::AuthHandler>(jwtObject));
    }

    static std::shared_ptr<OfferController> createShared(
            OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper),
            OATPP_COMPONENT(std::shared_ptr<auth::JWT>, jwt_)
    ) {
        return std::make_shared<OfferController>(objectMapper, jwt_);
    }

    ENDPOINT_INFO(createOne) {
        info->summary = "Create one offer";
        info->tags.emplace_back("offer-controller");
        info->addSecurityRequirement("JWT Bearer Auth", {});

        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_200, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_401, "application/json");
        info->addResponse<oatpp::Object<dto::StatusDto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "offer", createOne,
             AUTHORIZATION(std::shared_ptr<auth::JWT::Payload>, authObject),
             BODY_DTO(oatpp::Object<dto::OfferDto>, offerDto)) {
        auto dto = service_.createOne(offerDto);
        return createDtoResponse(Status::CODE_200, dto);
    }
};

#include OATPP_CODEGEN_END(ApiController)

} // namespace server::controller