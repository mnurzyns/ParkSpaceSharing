#include "service/user_service.hh"
#include "dto/offer_dto.hh"
#include <oatpp-sqlite/Utils.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

//NOLINTNEXTLINE
using Status = ::oatpp::web::protocol::http::Status;

namespace server::service
{

::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::user_dto>>>
user_service::get_users()
{
    auto res = database_->get_users();
    OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

    auto all = ::server::dto::page_dto<::oatpp::Object<::server::dto::user_dto>>::createShared();
    all->items = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::user_dto>>>();

    return all;
}

::oatpp::Object<::server::dto::user_dto>
user_service::get_user_byId(oatpp::UInt32 const& user_id)
{
    auto res = database_->get_user_byId(user_id);
    OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
    OATPP_ASSERT_HTTP(res->hasMoreToFetch(), Status::CODE_404, "User not found");

    auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::user_dto>>>();
    OATPP_ASSERT_HTTP(fetch->size() == 1, Status::CODE_500, "Unknown error");

    return fetch[0];
}

::oatpp::Object<::server::dto::user_dto>
user_service::create_user(::oatpp::Object<::server::dto::user_dto> const& dto)
{
    auto res = database_->create_user(dto);
    OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

    auto user_id = ::oatpp::sqlite::Utils::getLastInsertRowId(res->getConnection());
    return get_user_byId(static_cast<v_uint32>(user_id));
}

} // namespace server::service

