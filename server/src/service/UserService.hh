#pragma once

#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "auth/JWT.hh"
#include "database/MainDatabase.hh"
#include "dto/PageDto.hh"
#include "dto/UserDto.hh"

using Status = oatpp::web::protocol::http::Status;

namespace server::service {

class UserService {

private:
    OATPP_COMPONENT(std::shared_ptr<database::MainDatabase>, database);

public:
    static std::shared_ptr<UserService>
    createShared() {
        return std::make_shared<UserService>();
    }

    oatpp::Object<dto::UserDto>
    createOne(
        oatpp::Object<dto::UserDto> const& dto
    );

    oatpp::Object<dto::UserDto>
    getOne(
            oatpp::UInt64 const& userId
    );

    oatpp::Object<dto::PageDto<dto::UserDto>>
    searchUsers(
            oatpp::String const& query = "",
            oatpp::UInt64 const& limit = 20UL,
            oatpp::UInt64 const& offset = 0UL
    );

    oatpp::Object<dto::UserDto>
    updateOne(
        oatpp::UInt64 const& userId,
        oatpp::Object<dto::UserDto> const& dto
    );

    oatpp::Object<dto::UserDto>
    patchOne(
        oatpp::UInt64 const& userId,
        oatpp::Object<dto::UserDto> const& dto
    );

    oatpp::Object<dto::UserDto>
    deleteOne(
        oatpp::Int64 const& userId
    );
};

}  // namespace server::service
