#include "parkingSpace_service.hh"


//NOLINTNEXTLINE
using Status = ::oatpp::web::protocol::http::Status;

namespace server::service
{

    ::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::parkingSpace_dto>>>
    parkingSpace_service::get_parkingSpace() {
        auto res = database_->get_parkingSpace();
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

        auto all = ::server::dto::page_dto<::oatpp::Object<::server::dto::parkingSpace_dto>>::createShared();

        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::parkingSpace_dto>>>();
        OATPP_ASSERT_HTTP(!fetch->empty(), Status::CODE_404, "No parkingSpaces in db found");

        all->items = fetch;

        return all;
    }

    ::oatpp::Object<::server::dto::page_dto<::oatpp::Object<::server::dto::parkingSpace_dto>>>
    parkingSpace_service::get_myParkingSpace(oatpp::UInt32 const& user_id) {
        auto res = database_->get_myparkingSpace(user_id);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

        auto all = ::server::dto::page_dto<::oatpp::Object<::server::dto::parkingSpace_dto>>::createShared();

        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::parkingSpace_dto>>>();
        OATPP_ASSERT_HTTP(!fetch->empty(), Status::CODE_404, "No parkingSpaces found");

        all->items = fetch;

        return all;
    }

    ::oatpp::Object<::server::dto::parkingSpace_dto> 
    parkingSpace_service::get_parkingSpace_byId(oatpp::UInt32 const& parkingSpace_id){

        auto res = database_->get_parkingSpace_byId(parkingSpace_id);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(res->hasMoreToFetch(), Status::CODE_404, "parkingSpace not found");

        auto fetch = res->fetch<::oatpp::Vector<::oatpp::Object<::server::dto::parkingSpace_dto>>>();
        OATPP_ASSERT_HTTP(fetch->size() == 1, Status::CODE_500, "Unknown error");

        return fetch[0];
    }


    ::oatpp::Object<::server::dto::parkingSpace_dto> 
    parkingSpace_service::create_parkingSpace(::oatpp::Object<::server::dto::parkingSpace_dto> const& dto) {

        auto res = database_->is_parkingSpace_exist(dto->id);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        auto fetch = res->fetch<::oatpp::Vector<oatpp::UInt32>>();
        OATPP_ASSERT_HTTP(fetch->empty(), Status::CODE_409, "parking space is already exists");
    
        //Adding user to database
        res = database_->create_parkingSpace(dto);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());
        OATPP_ASSERT_HTTP(!res->hasMoreToFetch(), Status::CODE_404, "parkingSpace not created");

        auto ps_id = ::oatpp::sqlite::Utils::getLastInsertRowId(res->getConnection());

        return get_parkingSpace_byId(static_cast<v_uint32>(ps_id));
    }

    oatpp::Void
    parkingSpace_service::delete_parkingSpace(oatpp::UInt32 const& parkingSpace_id)
    {
        auto res = database_->delete_parkingSpace(parkingSpace_id);
        OATPP_ASSERT_HTTP(res->isSuccess(), Status::CODE_500, res->getErrorMessage());

        return nullptr;
    }

} // namespace server::service

