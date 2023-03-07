#include "service/offer_service.hh"
#include "dto/offer_dto.hh"
#include <oatpp/web/protocol/http/Http.hpp>

//NOLINTNEXTLINE
using Status = ::oatpp::web::protocol::http::Status;

namespace server::service
{

    ::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>>
    offer_service::get_offers() {
        auto res = database_->get_offers();
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

        auto all = ::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>::createShared();

        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::offer_dto>>>();
        OATPP_ASSERT_HTTP(!fetch->empty(), Status::CODE_404, "No offers found");

        all->items = fetch;

        return all;
    }

} // namespace server::service

