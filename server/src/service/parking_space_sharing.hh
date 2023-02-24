#pragma once

#include <memory>

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/parking_space_sharing.hh"
#include "dto/page.hh"
#include "dto/parking_space.hh"
#include "dto/user.hh"

namespace server::service
{

class parking_space_sharing
{
private:
    OATPP_COMPONENT(::std::shared_ptr<::server::database::parking_space_sharing>, database_);

public:
    ::oatpp::Object<::server::dto::all<::oatpp::Object<::server::dto::user>>>
    get_users();

    ::oatpp::Object<::server::dto::user>
    get_user(oatpp::UInt32 const& user_id);

    ::oatpp::Object<::server::dto::user>
    create_user(::oatpp::Object<::server::dto::user> const& dto);

    ::oatpp::Object<::server::dto::all<::oatpp::Object<::server::dto::parking_space>>>
    get_parking_spaces();
};

} // namespace server::service

