#include "TokenUtils.hh"

namespace server {

using std::chrono::system_clock, std::chrono::seconds;

TokenUtils::TokenUtils(std::string secret, std::string issuer)
  : secret_(std::move(secret))
  , issuer_(std::move(issuer))
  , verifier_(jwt::verify()
                .allow_algorithm(jwt::algorithm::hs512{ secret_ })
                .with_issuer(issuer_))
{
}

String
TokenUtils::createToken(std::shared_ptr<TokenPayload> payload)
{
    auto now = system_clock::now();
    auto token =
      jwt::create<JsonTraits>()
        .set_issuer(issuer_)
        .set_subject(std::to_string(payload->user_id))
        .set_issued_at(now)
        .set_not_before(now)
        .set_expires_at(now +
                        seconds{ Config::getInstance().jwt_expire_after })
        .set_type("JWS")
        .set_payload_claim("user_id", jwt::claim{ *payload->user_id })
        .set_payload_claim("role", jwt::claim{ payload->user_role })
        .sign(jwt::algorithm::hs512{ secret_ });

    return token;
}

TokenPayload
TokenUtils::readToken(jwt::decoded_jwt<JsonTraits> const& token)
{
    TokenPayload payload{};
    payload.user_id =
      static_cast<uint64_t>(token.get_payload_claim("user_id").as_int());
    payload.user_role = Role(token.get_payload_claim("role").as_int());

    return payload;
}

void
TokenUtils::verifyToken(jwt::decoded_jwt<JsonTraits> const& token) const
{
    verifier_.verify(token);
}

} // namespace server
