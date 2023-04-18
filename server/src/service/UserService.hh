#pragma once

#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

#include "database/MainDatabase.hh"
#include "database/model/UserModel.hh"
#include "dto/PageDto.hh"
#include "dto/UserDto.hh"
#include "auth/JWT.hh"

using Status = oatpp::web::protocol::http::Status;

namespace server::service {

class UserService {

private:
    OATPP_COMPONENT(std::shared_ptr<database::MainDatabase>, database);

public:
    static std::shared_ptr<UserService> createShared() {
        return std::make_shared<UserService>();
    }

    oatpp::Object<dto::UserDto> createOne(oatpp::Object<database::model::UserModel> const& model);

    oatpp::Object<dto::UserDto> getUser(oatpp::Int64 const& userId);

    oatpp::Object<dto::UserSearchPageDto> searchUsers(
        oatpp::String const& query = "",
        oatpp::UInt64 const& limit = 20ul,
        oatpp::UInt64 const& offset = 0ul
    );
};

}
