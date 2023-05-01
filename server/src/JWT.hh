#pragma once

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/defaults.h>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>

#include "Config.hh"

namespace server {

using namespace oatpp::data::mapping::type; // NOLINT
using JsonTraits = jwt::traits::nlohmann_json;

class JWT
{

  public:
    struct Payload : public oatpp::web::server::handler::AuthorizationObject
    {
        UInt64 user_id;
        Int32 role;
    };

  private:
    std::string secret_;
    std::string issuer_;
    jwt::verifier<jwt::default_clock, JsonTraits> verifier_;

  public:
    JWT(std::string secret, std::string issuer);

    String
    createToken(std::shared_ptr<Payload> payload);

    std::shared_ptr<Payload>
    readAndVerifyToken(String const& token);
};

} // namespace server
