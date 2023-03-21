#pragma once

#include <memory>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include "dto/page_dto.hh"
#include "dto/parkingSpace_dto.hh"
#include "dto/status_dto.hh"
//#include "service/_service.hh"

namespace server::controller
{

#include OATPP_CODEGEN_BEGIN(ApiController)

class parkingSpace_controller :
        public ::oatpp::web::server::api::ApiController
{
public:
    [[nodiscard]]
    explicit
    offer_controller(OATPP_COMPONENT(::std::shared_ptr<ObjectMapper>, object_mapper)) :
    ::oatpp::web::server::api::ApiController{object_mapper} {}

    static
    ::std::shared_ptr<parkingSpace_controller>
    create_shared(OATPP_COMPONENT(::std::shared_ptr<ObjectMapper>, object_mapper))
    {
        return ::std::make_shared<::server::controller::parkingSpace_controller>(object_mapper);
    }    
    


private:
    //::server::service::parkingSpace_service service_;
};

#include OATPP_CODEGEN_END(ApiController)

} // namespace server::controller