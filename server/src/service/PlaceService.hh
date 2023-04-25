#pragma once

#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "auth/JWT.hh"
#include "database/MainDatabase.hh"
#include "dto/PageDto.hh"
#include "dto/UserDto.hh"
#include "dto/StatusDto.hh"

using Status = oatpp::web::protocol::http::Status;

namespace server::service {

    class PlaceService {

    private:
        OATPP_COMPONENT(std::shared_ptr<database::MainDatabase>, database);

    public:
        static std::shared_ptr<PlaceService>
        createShared() {
            return std::make_shared<PlaceService>();
        }

        oatpp::Object<dto::PlaceDto>
        createOne(
                oatpp::Object<dto::PlaceDto> const &dto
        );

        oatpp::Object<dto::PlaceDto>
        getOne(
                oatpp::UInt64 const &placeId
        );

        //TODO:
        oatpp::Object<dto::PlacePageDto>
        search(
                oatpp::String const &query,
                oatpp::UInt64 const &limit = 20UL,
                oatpp::UInt64 const &offset = 0UL
        );

        oatpp::Object<dto::PlaceDto>
        putOne(
                oatpp::Object<dto::PlaceDto> const &dto
        );

        oatpp::Object<dto::PlaceDto>
        patchOne(
                oatpp::UInt64 const &placeId,
                oatpp::Object<dto::PlaceDto> const &dto
        );

        oatpp::Object<dto::StatusDto>
        deleteOne(
                oatpp::UInt64 const &placeId
        );
    };

}  // namespace server::service
