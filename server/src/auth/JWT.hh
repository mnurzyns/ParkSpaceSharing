#pragma once

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/boost-json/defaults.h>
#include <oatpp/web/server/handler/AuthorizationHandler.hpp>

using Traits = jwt::traits::boost_json;

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
        JWT(const oatpp::String &secret,
            const oatpp::String &issuer);

        oatpp::String createToken(const std::shared_ptr<Payload> &payload);

        std::shared_ptr<Payload> readAndVerifyToken(const oatpp::String &token);

    };

}  // namespace server::auth