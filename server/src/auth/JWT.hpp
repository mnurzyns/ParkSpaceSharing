
#include "oatpp/web/server/handler/AuthorizationHandler.hpp"
#include "oatpp/core/Types.hpp"

#include <jwt-cpp/jwt.h>

/*

class JWT {
public:

    struct Payload : public oatpp::web::server::handler::AuthorizationObject {

        oatpp::String userId;

    };

private:
    oatpp::String m_secret;
    oatpp::String m_issuer;
    jwt::verifier<jwt::default_clock, jwt::picojson_traits> m_verifier;
public:

    JWT(const oatpp::String& secret,
        const oatpp::String& issuer);

    oatpp::String createToken(const std::shared_ptr<Payload>& payload);

    std::shared_ptr<Payload> readAndVerifyToken(const oatpp::String& token);

};

*/