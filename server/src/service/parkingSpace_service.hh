#pragma once

#include <memory>

#include <oatpp-sqlite/Utils.hpp>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/pss_db.hh"
#include "dto/page_dto.hh"
#include "dto/status_dto.hh"
#include "dto/parkingSpace_dto.hh"

namespace server::service
{

    class parkingSpace_service
    {
    private:
        OATPP_COMPONENT(::std::shared_ptr<::server::database::pss_db>, database_);

    public:

        ::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::parkingSpace_dto>>>
        get_parkingSpace();

        ::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::parkingSpace_dto>>>
        get_myParkingSpace(oatpp::UInt32 const& user_id);

        ::oatpp::Object<::server::dto::parkingSpace_dto>
        get_parkingSpace_byId(oatpp::UInt32 const& id);

        ::oatpp::Object<::server::dto::parkingSpace_dto>
        create_parkingSpace(::oatpp::Object<::server::dto::parkingSpace_dto> const& dto);

        oatpp::Void
        delete_parkingSpace(oatpp::UInt32 const& id);

    };

} // namespace server::service

