#pragma once

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/defaults.h>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>

#include "Config.hh"

namespace server {

using oatpp::Void, oatpp::Any, oatpp::String, oatpp::Int8, oatpp::UInt8,
  oatpp::Int16, oatpp::UInt16, oatpp::Int32, oatpp::UInt32, oatpp::Int64,
  oatpp::UInt64, oatpp::Float32, oatpp::Float64, oatpp::Boolean, oatpp::Object,
  oatpp::Enum, oatpp::Vector, oatpp::List, oatpp::UnorderedSet,
  oatpp::UnorderedMap;

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
    String secret_;
    String issuer_;
    jwt::verifier<jwt::default_clock, JsonTraits> verifier_;

  public:
    JWT(String const& secret, String const& issuer);

    String
    createToken(std::shared_ptr<Payload> const& payload);

    std::shared_ptr<Payload>
    readAndVerifyToken(String const& token);
};

} // namespace server