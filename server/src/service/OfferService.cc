//namespace server::service
//{
//
//    ::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>>
//    offer_service::get_offers() {
//        auto res = database_->get_offers();
//        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
//        OATPP_ASSERT_HTTP(res->hasMoreToFetch(), Status::CODE_404, "No offers found");
//
//        auto all = ::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>::createShared();
//        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::offer_dto>>>();
//
//        all->items = fetch;
//
//        return all;
//    }
//
//    ::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>>
//    offer_service::get_myOffers(oatpp::UInt32 const& user_id) {
//        auto res = database_->get_myOffers(user_id);
//        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
//        OATPP_ASSERT_HTTP(res->hasMoreToFetch(), Status::CODE_404, "No offers found");
//
//        auto all = ::server::dto::page_dto<::oatpp::Object<::server::dto::offer_dto>>::createShared();
//        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::offer_dto>>>();
//
//        all->items = fetch;
//
//        return all;
//    }
//
//    ::oatpp::Object<::server::dto::offer_dto>
//    offer_service::get_offer_byId(oatpp::UInt32 const& offer_id){
//
//        auto res = database_->get_offer_byId(offer_id);
//        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
//        OATPP_ASSERT_HTTP(res->hasMoreToFetch(), Status::CODE_404, "Offer not found");
//
//        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::offer_dto>>>();
//        OATPP_ASSERT_HTTP(fetch->size() == 1, Status::CODE_500, "Unknown error");
//
//        return fetch[0];
//    }
//
//
//    ::oatpp::Object<::server::dto::offer_dto>
//    offer_service::create_offer(::oatpp::Object<::server::dto::offer_dto> const& dto, oatpp::UInt32 const& userId) {
//
//        auto res2 = database_->isHaveParkingSpace(userId,dto->parking_space_id);
//        OATPP_ASSERT_HTTP(res2->isSuccess(), Status::CODE_500, res2->getErrorMessage());
//        OATPP_ASSERT_HTTP(res2->hasMoreToFetch(), Status::CODE_409, "parking space isnt yours");
//
//        auto res = database_->is_offer_exist(dto->parking_space_id);
//        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
//        OATPP_ASSERT_HTTP(!res->hasMoreToFetch(), Status::CODE_409, "parking space is already offered");
//
//        res.reset();
//        res2.reset();
//
//        auto res3 = database_->create_offer(dto);
//        OATPP_ASSERT_HTTP(res3->isSuccess(), Status::CODE_500, res3->getErrorMessage());
//
//        auto offer_id = ::oatpp::sqlite::Utils::getLastInsertRowId(res3->getConnection());
//
//        return get_offer_byId(static_cast<v_uint32>(offer_id));
//    }
//
//    oatpp::String
//    offer_service::delete_myOffer(oatpp::UInt32 const& offer_id,oatpp::UInt32 const& user_id)
//    {
//        auto res = database_->delete_myOffer(offer_id,user_id);
//        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
//
//        return "OK";
//    }
//
//    oatpp::String
//    offer_service::delete_offer(oatpp::UInt32 const& offer_id)
//    {
//        auto res = database_->delete_offer(offer_id);
//        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
//
//        return "OK";
//    }
//
//} // namespace server::service
#include "OfferService.hh"
//TODO: implement filtering by properties of associated props
auto server::service::OfferService::searchOffers(
        const oatpp::web::protocol::http::QueryParams &queryParams,
        const oatpp::UInt64 &limit,
        const oatpp::UInt64 &offset
) {
    std::string query = "SELECT * FROM offer";

    auto

    if (queryParams.get("")) {
        query += " WHERE parking_space_id = " + queryParams.get("parking_space_id");
    }

    return nullptr;
}
