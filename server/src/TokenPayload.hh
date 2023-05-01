#pragma once

#include <oatpp/core/Types.hpp>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>

#include "dto/UserDto.hh"

namespace server {

using namespace oatpp::data::mapping::type; // NOLINT
using dto::Role;                            // NOLINT

struct TokenPayload : public oatpp::web::server::handler::AuthorizationObject
{
    UInt64 user_id;
    Role user_role;
};

}
