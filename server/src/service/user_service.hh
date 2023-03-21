#pragma once

#include <memory>

#include <oatpp-sqlite/Utils.hpp>
#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/pss_db.hh"
#include "dto/page_dto.hh"
#include "dto/user_dto.hh"

namespace server::service
{

    class user_service
    {
    private:
        OATPP_COMPONENT(::std::shared_ptr<::server::database::pss_db>, database_);

    public:
        ::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::user_dto>>>
        get_users();

        ::oatpp::Object<::server::dto::user_dto>
        get_user_byId(oatpp::UInt32 const& user_id);

        ::oatpp::Object<::server::dto::user_dto>
        create_user(::oatpp::Object<::server::dto::user_dto> const& dto);
    };

} // namespace server::service

