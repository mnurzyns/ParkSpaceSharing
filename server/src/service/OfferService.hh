#pragma once

#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>
#include "auth/JWT.hh"
#include "database/MainDatabase.hh"

namespace server::service
{

    class OfferService
    {
    private:
        OATPP_COMPONENT(std::shared_ptr<database::MainDatabase>, database_);

    public:
        auto searchOffers(
            oatpp::web::protocol::http::QueryParams const& queryParams,
            oatpp::UInt64 const& limit = 20UL,
            oatpp::UInt64 const& offset = 0UL
        );
    };

} // namespace server::service

