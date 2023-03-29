
#ifndef JWT_HPP
#define JWT_HPP

#include <oatpp/web/server/handler/AuthorizationHandler.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/Types.hpp>

#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

#include "dto/user_dto.hh"


using traits = jwt::traits::nlohmann_json;
//using claim = jwt::basic_claim<traits>;

class JWT {
public:

    struct Payload : public oatpp::web::server::handler::AuthorizationObject {
        ::oatpp::Object<::server::dto::user_dto> user;
    };

private:
    oatpp::String m_secret;
    oatpp::String m_issuer;
    jwt::verifier<jwt::default_clock, traits> m_verifier;
    oatpp::parser::json::mapping::ObjectMapper m_mapper;
public:

    JWT(const oatpp::String& secret,
        const oatpp::String& issuer);

    ::oatpp::String createToken(const std::shared_ptr<Payload>& payload);

    std::shared_ptr<Payload> readAndVerifyToken(const ::oatpp::String& token);

};

#endif