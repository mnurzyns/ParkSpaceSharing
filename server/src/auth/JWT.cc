#include <oatpp/core/Types.hpp>

#include "JWT.hh"

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
        auto token = jwt::create<Traits>()
                .set_issuer(issuer_)
                .set_subject(std::to_string(payload->userId))
                .set_issued_at(std::chrono::system_clock::now())
                .set_not_before(std::chrono::system_clock::now())
                .set_expires_at(std::chrono::system_clock::now() + std::chrono::days{30}) //TODO: move to config
                .set_type("JWS")
                .set_payload_claim("userId", boost::json::value(payload->userId))
                .set_payload_claim("role", boost::json::value(payload->role))
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
