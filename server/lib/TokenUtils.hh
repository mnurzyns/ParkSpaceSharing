#pragma once

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/defaults.h>
#include <oatpp/core/Types.hpp>

#include "Config.hh"
#include "TokenPayload.hh"

namespace server {

using namespace oatpp::data::mapping::type; // NOLINT
using JsonTraits = jwt::traits::nlohmann_json;

class TokenUtils
{

  private:
    std::string secret_;
    std::string issuer_;
    jwt::verifier<jwt::default_clock, JsonTraits> verifier_;

  public:
    TokenUtils(std::string secret, std::string issuer);

    String
    createToken(std::shared_ptr<TokenPayload> payload);

    static TokenPayload
    readToken(jwt::decoded_jwt<JsonTraits> const& token);

    void
    verifyToken(jwt::decoded_jwt<JsonTraits> const& token) const;
};

} // namespace server
