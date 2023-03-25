
#ifndef EXAMPLE_JWT_JWT_HPP
#define EXAMPLE_JWT_JWT_HPP

#include <oatpp/web/server/handler/AuthorizationHandler.hpp>
#include <oatpp/core/Types.hpp>

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>
#include <string>


using traits = jwt::traits::nlohmann_json;
//using claim = jwt::basic_claim<traits>;

class JWT {
public:

    struct Payload : public oatpp::web::server::handler::AuthorizationObject {
        ::oatpp::UInt64 userId;
    };

private:
    oatpp::String m_secret;
    oatpp::String m_issuer;
    jwt::verifier<jwt::default_clock, traits> m_verifier;
public:

    JWT(const oatpp::String& secret,
        const oatpp::String& issuer);

    ::oatpp::String createToken(const std::shared_ptr<Payload>& payload);

    std::shared_ptr<Payload> readAndVerifyToken(const ::oatpp::String& token);

};

#endif