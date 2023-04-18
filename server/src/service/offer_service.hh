#pragma once

#include <memory>

#include <oatpp-sqlite/Utils.hpp>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/pss_db.hh"
#include "dto/page_dto.hh"
#include "dto/offer_dto.hh"
#include "dto/status_dto.hh"

namespace server::service
{

    class offer_service
    {
    private:
        OATPP_COMPONENT(::std::shared_ptr<::server::database::pss_db>, database_);

    public:
        oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>>
        get_offers();

        oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>>
        get_myOffers(oatpp::UInt32 const& user_id);

        oatpp::Object<::server::dto::offer_dto> 
        get_offer_byId(oatpp::UInt32 const& offer_id);

        oatpp::Object<::server::dto::offer_dto>
        create_offer(::oatpp::Object<::server::dto::offer_dto> const& dto,::oatpp::UInt32 const& userId);

        oatpp::String
        delete_myOffer(oatpp::UInt32 const& offer_id,oatpp::UInt32 const& user_id);

        oatpp::String
        delete_offer(oatpp::UInt32 const& offer_id);
    };

} // namespace server::service

