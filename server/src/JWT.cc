#include "JWT.hh"

namespace server {

using std::chrono::system_clock, std::chrono::milliseconds;

JWT::JWT(String const& secret, String const& issuer)
  : secret_(secret)
  , issuer_(issuer)
  , verifier_(jwt::verify()
                .allow_algorithm(jwt::algorithm::hs512{ secret })
                .with_issuer(issuer))
{
}

String
JWT::createToken(std::shared_ptr<Payload> const& payload)
{
    auto now = system_clock::now();
    auto token =
      jwt::create<JsonTraits>()
        .set_issuer(issuer_)
        .set_subject(std::to_string(payload->user_id))
        .set_issued_at(now)
        .set_not_before(now)
        .set_expires_at(now +
                        milliseconds{ Config::getInstance().jwt_expire_after })
        .set_type("JWS")
        .set_payload_claim("user_id", jwt::claim{ *payload->user_id })
        .set_payload_claim("role", jwt::claim{ *payload->role })
        .sign(jwt::algorithm::hs512{ secret_ });

    return token;
}

std::shared_ptr<JWT::Payload>
JWT::readAndVerifyToken(String const& token)
{
    auto decoded = jwt::decode<JsonTraits>(token);
    verifier_.verify(decoded);

    // TODO: Verify that the issuer exists in the database (custom verifier?)

    auto payload = std::make_shared<Payload>();
    payload->user_id =
      static_cast<uint64_t>(decoded.get_payload_claim("user_id").as_int());
    payload->role =
      static_cast<int32_t>(decoded.get_payload_claim("role").as_int());

    return payload;
}

}  // namespace server
