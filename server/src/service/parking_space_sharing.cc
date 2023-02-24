#include "service/parking_space_sharing.hh"
#include "dto/parking_space.hh"
#include <oatpp-sqlite/Utils.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

//NOLINTNEXTLINE
using Status = ::oatpp::web::protocol::http::Status;

namespace server::service
{

::oatpp::Object<::server::dto::all<::oatpp::Object<::server::dto::user>>>
parking_space_sharing::get_users()
{
    auto res = database_->get_users();
    OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

    auto all = ::server::dto::all<::oatpp::Object<::server::dto::user>>::createShared();
    all->items = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::user>>>();

    return all;
}

::oatpp::Object<::server::dto::user>
parking_space_sharing::get_user(oatpp::UInt32 const& user_id)
{
    auto res = database_->get_user(user_id);
    OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
    OATPP_ASSERT_HTTP(res->hasMoreToFetch(), Status::CODE_404, "User not found");

    auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::user>>>();
    OATPP_ASSERT_HTTP(fetch->size() == 1, Status::CODE_500, "Unknown error");

    return fetch[0];
}

::oatpp::Object<::server::dto::user>
parking_space_sharing::create_user(::oatpp::Object<::server::dto::user> const& dto)
{
    auto res = database_->create_user(dto);
    OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

    auto user_id = ::oatpp::sqlite::Utils::getLastInsertRowId(res->getConnection());
    return get_user(static_cast<v_uint32>(user_id));
}

::oatpp::Object<::server::dto::all<::oatpp::Object<::server::dto::parking_space>>>
parking_space_sharing::get_parking_spaces()
{
    auto res = database_->get_parking_spaces();
    OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

    auto all = ::server::dto::all<::oatpp::Object<::server::dto::parking_space>>::createShared();

    auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::parking_space>>>();
    OATPP_ASSERT_HTTP(!fetch->empty(), Status::CODE_404, "No parking spaces found");

    all->items = fetch;

    return all;
}

} // namespace server::service

