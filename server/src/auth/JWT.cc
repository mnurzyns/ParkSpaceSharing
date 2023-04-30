#include "JWT.hh"

#include <oatpp/core/Types.hpp>

#include "config.hh"

namespace server::auth {

    JWT::JWT(
            oatpp::String const &secret,
            oatpp::String const &issuer
    ) : secret_(secret), issuer_(issuer),
        verifier_(jwt::verify().allow_algorithm(jwt::algorithm::hs512{secret}).with_issuer(issuer)) {}

    oatpp::String
    JWT::createToken(
            std::shared_ptr<Payload> const &payload
    ) {
        auto now = std::chrono::system_clock::now();
        auto token = jwt::create<Traits>()
                .set_issuer(issuer_)
                .set_subject(std::to_string(payload->userId))
                .set_issued_at(now)
                .set_not_before(now)
                .set_expires_at(now + std::chrono::seconds{server::config::get_instance().jwt_expire_after})
                .set_type("JWS")
                .set_payload_claim("userId", jwt::claim{*payload->userId})
                .set_payload_claim("role", jwt::claim{*payload->role})
                .sign(jwt::algorithm::hs512{secret_});

        return token;
    }

    std::shared_ptr<JWT::Payload>
    JWT::readAndVerifyToken(
            oatpp::String const &token
    ) {
        auto decoded = jwt::decode<Traits>(token);
        verifier_.verify(decoded);

        //TODO: Verify that the issuer exists in the database (custom verifier?)

        auto payload = std::make_shared<Payload>();
        payload->userId = static_cast<uint64_t>(decoded.get_payload_claim("userId").as_int());
        payload->role = static_cast<int32_t>(decoded.get_payload_claim("role").as_int());

        return payload;
    }

}  // namespace server::auth
