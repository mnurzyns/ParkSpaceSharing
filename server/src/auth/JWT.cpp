#include "auth/JWT.hpp"
#include <cstdint>
#include <jwt-cpp/jwt.h>
#include <oatpp/core/Types.hpp>


JWT::JWT(const oatpp::String& secret,
  const oatpp::String& issuer)
  : m_secret_(secret)
  , m_issuer_(issuer)
  , m_verifier_(jwt::verify<traits>()
  .allow_algorithm(jwt::algorithm::hs512{ secret })
  .with_issuer(issuer))
{}

::oatpp::String JWT::createToken(const std::shared_ptr<payload>& m_payload) {
  auto token = jwt::create<traits>()
    .set_issuer(m_issuer_)
    .set_type("JWS")
    .set_payload_claim("userId", jwt::basic_claim<traits>(traits::json::number_integer_t(m_payload->userId)))
    .set_payload_claim("isAdmin", jwt::basic_claim<traits>(traits::json::boolean_t(m_payload->isAdmin)))
    .sign(jwt::algorithm::hs512{ m_secret_ });

    
  return token;
}

std::shared_ptr<JWT::payload> JWT::readAndVerifyToken(const oatpp::String& token) {

  auto decoded = jwt::decode<traits>(token);
  m_verifier_.verify(decoded);

  auto m_payload = std::make_shared<payload>();
  m_payload->userId = decoded.get_payload_claims().at("userId").as_();
  m_payload->isAdmin = decoded.get_payload_claims().at("isAdmin").as_bool();

  return m_payload;
}


