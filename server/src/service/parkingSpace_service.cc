#include "service/parkingSpace_service.hh"


//NOLINTNEXTLINE
using Status = ::oatpp::web::protocol::http::Status;

namespace server::service
{

    ::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::parkingSpace_dto>>>
    parkingSpace_service::get_parkingSpace() {
        auto res = database_->get_offers();
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

        auto all = ::server::dto::page_dto<::oatpp::Object<::server::dto::parkingSpace_dto>>::createShared();

        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::parkingSpace_dto>>>();
        OATPP_ASSERT_HTTP(!fetch->empty(), Status::CODE_404, "No offers found");

        all->items = fetch;

        return all;
    }

    ::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::parkingSpace_dto>>>
    parkingSpace_service::get_myParkingSpace(oatpp::UInt32 const& user_id) {
        auto res = database_->get_myOffers(user_id);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

        auto all = ::server::dto::page_dto<::oatpp::Object<::server::dto::parkingSpace_dto>>::createShared();

        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::parkingSpace_dto>>>();
        OATPP_ASSERT_HTTP(!fetch->empty(), Status::CODE_404, "No offers found");

        all->items = fetch;

        return all;
    }

    ::oatpp::Object<::server::dto::parkingSpace_dto> 
    parkingSpace_service::get_parkingSpace_byId(oatpp::UInt32 const& id){

        auto res = database_->get_offer_byId(id);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(res->hasMoreToFetch(), Status::CODE_404, "Offer not found");

        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::parkingSpace_dto>>>();
        OATPP_ASSERT_HTTP(fetch->size() == 1, Status::CODE_500, "Unknown error");

        return fetch[0];
    }


    ::oatpp::Object<::server::dto::parkingSpace_dto> 
    parkingSpace_service::create_parkingSpace(::oatpp::Object<::server::dto::parkingSpace_dto> const& dto) {

        auto res = database_->is_offer_exist(dto->id_parking_space);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        auto fetch = res->fetch<::oatpp::Vector<oatpp::UInt32>>();
        OATPP_ASSERT_HTTP(fetch->size() == 0, Status::CODE_409, "parking space is already offered");
    
        //Adding user to database
        res = database_->create_offer(dto);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(!res->hasMoreToFetch(), Status::CODE_404, "offer not created");

        auto id = ::oatpp::sqlite::Utils::getLastInsertRowId(res->getConnection());

        return get_parkingSpace_byId(static_cast<v_uint32>(id));
    }

} // namespace server::service

