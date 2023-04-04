
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

    struct payload : public oatpp::web::server::handler::AuthorizationObject {
        ::oatpp::Int32 userId;
        ::oatpp::Boolean isAdmin;
    };

private:
    oatpp::String m_secret_;
    oatpp::String m_issuer_;
    jwt::verifier<jwt::default_clock, traits> m_verifier_;
public:

    JWT(const oatpp::String& secret,
        const oatpp::String& issuer);

    ::oatpp::String createToken(const std::shared_ptr<payload>& m_payload);

    std::shared_ptr<payload> readAndVerifyToken(const ::oatpp::String& token);

};

#endif