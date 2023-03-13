#pragma once

#include "dto/page_dto.hh"
#include "dto/offer_dto.hh"
#include "dto/status_dto.hh"
#include "service/offer_service.hh"

#include <memory>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

namespace server::controller
{

#include OATPP_CODEGEN_BEGIN(ApiController)

class offer_controller :
        public ::oatpp::web::server::api::ApiController
{
public:
    [[nodiscard]]
    explicit
    offer_controller(OATPP_COMPONENT(::std::shared_ptr<ObjectMapper>, object_mapper)) :
    ::oatpp::web::server::api::ApiController{object_mapper} {}

    static
    ::std::shared_ptr<offer_controller>
    create_shared(OATPP_COMPONENT(::std::shared_ptr<ObjectMapper>, object_mapper))
    {
        return ::std::make_shared<::server::controller::offer_controller>(object_mapper);
    }
    
    ENDPOINT_INFO(get_offers)
    {
        info->summary = "Get page_dto parking spaces";
        info->tags.emplace_back("offer-controller");

        info->addResponse<::oatpp::Object<::server::dto::offer_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_404, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "offers", get_offers)
    {
        return createDtoResponse(Status::CODE_200, service_.get_offers());
    }

private:
    ::server::service::offer_service service_;
};

#include OATPP_CODEGEN_END(ApiController)

} // namespace server::controller