#pragma once

#include <oatpp/web/server/handler/AuthorizationHandler.hpp>
#include <oatpp/core/Types.hpp>

namespace server {

using namespace oatpp::data::mapping::type; // NOLINT

struct TokenPayload : public oatpp::web::server::handler::AuthorizationObject
{
    UInt64 user_id;
    Int32 role;
};

}
