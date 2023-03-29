#include "auth/JWT.hpp"


JWT::JWT(const oatpp::String& secret,
  const oatpp::String& issuer)
  : m_secret(secret)
  , m_issuer(issuer)
  , m_verifier(jwt::verify<traits>()
  .allow_algorithm(jwt::algorithm::hs256{ secret })
  .with_issuer(issuer))
{}

::oatpp::String JWT::createToken(const std::shared_ptr<Payload>& payload) {
  auto token = jwt::create<traits>()
    .set_issuer(m_issuer)
    .set_type("JWS")

    .set_payload_claim("user", jwt::basic_claim<traits>(std::string{m_mapper.writeToString(payload->user)}))
    
    .sign(jwt::algorithm::hs256{m_secret}
  );

  return token;
}

std::shared_ptr<JWT::Payload> JWT::readAndVerifyToken(const oatpp::String& token) {

  auto decoded = jwt::decode<traits>(token);
  m_verifier.verify(decoded);

  auto payload = std::make_shared<Payload>();
  payload->user = m_mapper.readFromString<::oatpp::Object<::server::dto::user_dto>>(decoded.get_payload_claims().at("user").as_string());

  return payload;
}


