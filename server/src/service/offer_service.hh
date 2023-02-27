#pragma once

#include <memory>

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/pss_db.hh"
#include "dto/page_dto.hh"
#include "dto/offer_dto.hh"
#include "dto/user_dto.hh"

namespace server::service
{

    class offer_service
    {
    private:
        OATPP_COMPONENT(::std::shared_ptr<::server::database::pss_db>, database_);

    public:
        ::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>>
        get_offers();
    };

} // namespace server::service

