#pragma once

#include <memory>

#include <oatpp-sqlite/Utils.hpp>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/pss_db.hh"
#include "dto/page_dto.hh"
#include "dto/status_dto.hh"

namespace server::service
{

    class parkingSpace_service
    {
    private:
        OATPP_COMPONENT(::std::shared_ptr<::server::database::pss_db>, database_);

    public:


    };

} // namespace server::service

