#pragma once

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/defaults.h>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>

using Traits = jwt::traits::nlohmann_json;

namespace server::auth {

    class JWT {

    public:
        struct Payload : public oatpp::web::server::handler::AuthorizationObject {
            oatpp::UInt64 userId;
            oatpp::Int32 role;
        };

    private:
        oatpp::String secret_;
        oatpp::String issuer_;
        jwt::verifier<jwt::default_clock, Traits> verifier_;

    public:
        JWT(
                oatpp::String const &secret,
                oatpp::String const &issuer
        );

        oatpp::String
        createToken(
                std::shared_ptr<Payload> const &payload
        );

        std::shared_ptr<Payload>
        readAndVerifyToken(
                oatpp::String const &token
        );

    };

}  // namespace server::auth
