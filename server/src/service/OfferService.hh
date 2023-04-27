#pragma once

#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/MainDatabase.hh"
#include "dto/PlaceDto.hh"
#include "dto/StatusDto.hh"

using Status = oatpp::web::protocol::http::Status;

namespace server::service {

    class OfferService {

    private:
        OATPP_COMPONENT(std::shared_ptr<database::MainDatabase>, database_);

    public:
        static std::shared_ptr<OfferService>
        createShared() {
            return std::make_shared<OfferService>();
        }

        oatpp::Object<dto::OfferDto>
        createOne(
                oatpp::Object<dto::OfferDto> const &dto
        );

        oatpp::Object<dto::OfferDto>
        getOne(
                oatpp::UInt64 const &id
        );

        oatpp::Object<dto::OfferPageDto>
        search(
                oatpp::String const &query,
                oatpp::UInt64 const &limit = 20UL,
                oatpp::UInt64 const &offset = 0UL
        );

        oatpp::Object<dto::OfferDto>
        putOne(
                oatpp::Object<dto::OfferDto> const &dto
        );

        oatpp::Object<dto::OfferDto>
        patchOne(
                oatpp::UInt64 const &id,
                oatpp::Object<dto::OfferDto> const &dto
        );

        oatpp::Object<dto::StatusDto>
        deleteOne(
                oatpp::UInt64 const &id
        );
    };

}  // namespace server::service
