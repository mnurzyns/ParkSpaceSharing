#include <oatpp/core/Types.hpp>

#include "JWT.hh"

namespace server::auth {

JWT::JWT(const oatpp::String& secret,
         const oatpp::String& issuer)
    : secret_(secret)
    , issuer_(issuer)
    , m_verifier_(jwt::verify()
        .allow_algorithm(jwt::algorithm::hs512{ secret })
        .with_issuer(issuer))
{}

oatpp::String JWT::createToken(const std::shared_ptr<Payload>& payload) {
    auto token = jwt::create<Traits>()
        .set_issuer(issuer_)
        .set_type("JWS")

        .set_payload_claim("userId", boost::json::value(uint64_t{payload->userId}))
        .set_payload_claim("isAdmin", boost::json::value(bool{payload->isAdmin}))
        .sign(jwt::algorithm::hs512{ secret_ });

    return token;
}

std::shared_ptr<JWT::Payload> JWT::readAndVerifyToken(const oatpp::String& token) {

    auto decoded = jwt::decode<Traits>(token);
    m_verifier_.verify(decoded);

    auto m_payload = std::make_shared<Payload>();
    m_payload->userId =  static_cast<uint64_t>(decoded.get_payload_claim("userId").as_int());
    m_payload->isAdmin = decoded.get_payload_claim("isAdmin").as_bool();

    return m_payload;
}

}  // namespace server::auth
