#include "service/offer_service.hh"


//NOLINTNEXTLINE
using Status = ::oatpp::web::protocol::http::Status;

namespace server::service
{

    ::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>>
    offer_service::get_offers() {
        auto res = database_->get_offers();
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(!res->hasMoreToFetch(), Status::CODE_404, "No offers found");

        auto all = ::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>::createShared();
        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::offer_dto>>>();

        all->items = fetch;

        return all;
    }

    ::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>>
    offer_service::get_myOffers(oatpp::UInt32 const& user_id) {
        auto res = database_->get_myOffers(user_id);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(!res->hasMoreToFetch(), Status::CODE_404, "No offers found");

        auto all = ::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>::createShared();
        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::offer_dto>>>();

        all->items = fetch;

        return all;
    }

    ::oatpp::Object<::server::dto::offer_dto> 
    offer_service::get_offer_byId(oatpp::UInt32 const& offer_id){

        auto res = database_->get_offer_byId(offer_id);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(res->hasMoreToFetch(), Status::CODE_404, "Offer not found");

        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::offer_dto>>>();
        OATPP_ASSERT_HTTP(fetch->size() == 1, Status::CODE_500, "Unknown error");

        return fetch[0];
    }


    ::oatpp::Object<::server::dto::offer_dto> 
    offer_service::create_offer(::oatpp::Object<::server::dto::offer_dto> const& dto, oatpp::UInt32 const& userId) {

        auto res = database_->is_offer_exist(dto->id_parkingSpace);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(!res->hasMoreToFetch(), Status::CODE_409, "parking space is already offered");

        res = database_->isHaveParkingSpace(userId,dto->id_parkingSpace);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(!res->hasMoreToFetch(), Status::CODE_409, "parking space isnt ours");
    
        //Adding offer to database
        res = database_->create_offer(dto);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

        auto offer_id = ::oatpp::sqlite::Utils::getLastInsertRowId(res->getConnection());

        return get_offer_byId(static_cast<v_uint32>(offer_id));
    }

    oatpp::Void
    offer_service::delete_myOffer(oatpp::UInt32 const& offer_id,oatpp::UInt32 const& user_id)
    {
        auto res = database_->delete_myOffer(offer_id,user_id);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

        return nullptr;
    }

    oatpp::Void
    offer_service::delete_offer(oatpp::UInt32 const& offer_id)
    {
        auto res = database_->delete_offer(offer_id);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

        return nullptr;
    }

} // namespace server::service

