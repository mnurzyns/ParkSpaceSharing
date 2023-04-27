#pragma once

#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/MainDatabase.hh"
#include "dto/UserDto.hh"
#include "dto/StatusDto.hh"

namespace server::service {

    using Status = oatpp::web::protocol::http::Status;

    class UserService {

    private:
        OATPP_COMPONENT(std::shared_ptr<database::MainDatabase>, database_);

    public:
        static std::shared_ptr<UserService>
        createShared();

        oatpp::Object<dto::UserDto>
        createOne(
                oatpp::Object<dto::UserDto> const &dto
        );

        oatpp::Object<dto::UserDto>
        getOne(
                oatpp::UInt64 const &id
        );

        oatpp::Object<dto::UserPageDto>
        search(
                oatpp::String const &query,
                oatpp::UInt64 const &limit = 20UL,
                oatpp::UInt64 const &offset = 0UL
        );

        oatpp::Object<dto::UserDto>
        putOne(
                oatpp::Object<dto::UserDto> const &dto
        );

        oatpp::Object<dto::UserDto>
        patchOne(
                oatpp::UInt64 const &id,
                oatpp::Object<dto::UserDto> const &dto
        );

        oatpp::Object<dto::StatusDto>
        deleteOne(
                oatpp::UInt64 const &id
        );
    };

}  // namespace server::service
