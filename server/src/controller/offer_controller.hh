#pragma once

#include "dto/page_dto.hh"
#include "dto/offer_dto.hh"
#include "dto/status_dto.hh"
#include "database/pss_db.hh"
#include "service/offer_service.hh"

#include <cstddef>
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
        info->summary = "Get page_dto of all offers";
        info->tags.emplace_back("offer_controller");

        info->addResponse<::oatpp::Object<::server::dto::offer_page_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_404, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "offers", get_offers)
    {
        return createDtoResponse(Status::CODE_200, service_.get_offers());
    }

    ENDPOINT_INFO(get_myOffers)
    {
        info->summary = "Get page_dto of my offerts";
        info->tags.emplace_back("offer_controller");

        info->addResponse<::oatpp::Object<::server::dto::offer_page_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_404, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "user/offers", get_myOffers, BUNDLE(::oatpp::Object<::server::dto::user_dto>, user))
    {
        return createDtoResponse(Status::CODE_200, service_.get_myOffers(user->id));
    }

    ENDPOINT_INFO(get_offer_byId)
    {
        info->summary = "Get offer by id";
        info->tags.emplace_back("offer_controller");

        info->addResponse<::oatpp::Object<::server::dto::offer_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_404, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "offers/{offer_id}", get_offer_byId, PATH(oatpp::UInt32, offer_id))
    {
        return createDtoResponse(Status::CODE_200, service_.get_offer_byId(offer_id));
    }

    ENDPOINT_INFO(create_offer)
    {
        info->summary = "Create a new offer";
        info->tags.emplace_back("offer_controller");

        info->addResponse<::oatpp::Object<::server::dto::offer_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_404, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "offers", create_offer, BODY_DTO(Object<::server::dto::offer_dto>, offer_dto), BUNDLE(::oatpp::Object<::server::dto::user_dto>, user)) 
    {
        return createDtoResponse(Status::CODE_200, service_.create_offer(offer_dto,user->id));
    }
    
    ENDPOINT_INFO(delete_myOffer) {
        info->summary = "Delete myOffer";
        info->tags.emplace_back("offer_controller");

        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_200, "application/json");

    }
    ENDPOINT("DELETE", "user/offer/{offer_id}", delete_myOffer, PATH(oatpp::UInt32, offer_id), BUNDLE(::oatpp::Object<::server::dto::user_dto>, user) )
    {
        return createDtoResponse(Status::CODE_200, service_.delete_myOffer(offer_id,user->id));
    }

    ENDPOINT_INFO(delete_offer) {
        info->summary = "Delete offer (for admin use)";
        info->tags.emplace_back("offer_controller");

        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_200, "application/json");

    }
    ENDPOINT("DELETE", "offer/{offer_id}", delete_offer, PATH(oatpp::UInt32, offer_id))
    {
        return createDtoResponse(Status::CODE_200, service_.delete_offer(offer_id));
    }    
    

private:
    ::server::service::offer_service service_;
};

#include OATPP_CODEGEN_END(ApiController)

} // namespace server::controller