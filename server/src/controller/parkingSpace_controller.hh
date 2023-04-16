#pragma once

#include "dto/page_dto.hh"
#include "dto/offer_dto.hh"
#include "dto/status_dto.hh"
#include "database/pss_db.hh"
#include "service/parkingSpace_service.hh"

#include <memory>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

namespace server::controller
{

#include OATPP_CODEGEN_BEGIN(ApiController)

class parkingSpace_controller :
        public ::oatpp::web::server::api::ApiController
{
public:
    [[nodiscard]]
    explicit
    parkingSpace_controller(OATPP_COMPONENT(::std::shared_ptr<ObjectMapper>, object_mapper)) :
    ::oatpp::web::server::api::ApiController{object_mapper} {}

    static
    ::std::shared_ptr<parkingSpace_controller>
    create_shared(OATPP_COMPONENT(::std::shared_ptr<ObjectMapper>, object_mapper))
    {
        return ::std::make_shared<::server::controller::parkingSpace_controller>(object_mapper);
    }
    
    ENDPOINT_INFO(get_parkingSpace)
    {
        info->summary = "Get page_dto of all parkingSpace (for admin use)";
        info->tags.emplace_back("parkingSpace_controller");

        info->addResponse<::oatpp::Object<::server::dto::parkingSpace_page_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_404, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "parkingSpace", get_parkingSpace)
    {
        return createDtoResponse(Status::CODE_200, service_.get_parkingSpace());
    }
    ENDPOINT_INFO(get_myParkingSpace)
    {
        info->summary = "Get page_dto of my offerts";
        info->tags.emplace_back("parkingSpace_controller");

        info->addResponse<::oatpp::Object<::server::dto::parkingSpace_page_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_404, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "user/parkingSpace", get_myParkingSpace, BUNDLE(::oatpp::Object<::server::dto::user_dto>, user) )
    {
        return createDtoResponse(Status::CODE_200, service_.get_myParkingSpace(user->id));
    }
    

    ENDPOINT_INFO(get_parkingSpace_byId)
    {
        info->summary = "Get parkingSpace by id (for admin use)";
        info->tags.emplace_back("parkingSpace_controller");

        info->addResponse<::oatpp::Object<::server::dto::parkingSpace_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_404, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("GET", "parkingSpace/{parkingSpace_id}", get_parkingSpace_byId, PATH(oatpp::UInt32, parkingSpace_id))
    {
        return createDtoResponse(Status::CODE_200, service_.get_parkingSpace_byId(parkingSpace_id));
    }

    ENDPOINT_INFO(create_parkingSpace)
    {
        info->summary = "Create a new parkingSpace (for admin use)";
        info->tags.emplace_back("parkingSpace_controller");

        info->addResponse<::oatpp::Object<::server::dto::parkingSpace_dto>>(Status::CODE_200, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_404, "application/json");
        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_500, "application/json");
    }
    ENDPOINT("POST", "parkingSpace", create_parkingSpace, BODY_DTO(Object<::server::dto::parkingSpace_dto>, parkingSpace_dto)) 
    {
        return createDtoResponse(Status::CODE_200, service_.create_parkingSpace(parkingSpace_dto));
    }  

    
    ENDPOINT_INFO(delete_parkingSpace) {
        info->summary = "Delete parkingSpace (for admin use)";
        info->tags.emplace_back("parkingSpace_controller");

        info->addResponse<::oatpp::Object<::server::dto::status_dto>>(Status::CODE_200, "application/json");

    }
    ENDPOINT("DELETE", "parkingSpace/{parkingSpace_id}", delete_parkingSpace, PATH(oatpp::UInt32, parkingSpace_id))
    {
        return createDtoResponse(Status::CODE_200, service_.delete_parkingSpace(parkingSpace_id));
    }



private:
    ::server::service::parkingSpace_service service_;
};

#include OATPP_CODEGEN_END(ApiController)

} // namespace server::controller