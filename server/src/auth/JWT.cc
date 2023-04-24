#include <oatpp/core/Types.hpp>

#include "JWT.hh"

namespace server::auth {

    JWT::JWT(const oatpp::String &secret,
             const oatpp::String &issuer)
            : secret_(secret), issuer_(issuer),
              verifier_(jwt::verify().allow_algorithm(jwt::algorithm::hs512{secret}).with_issuer(issuer)) {}

    oatpp::String JWT::createToken(const std::shared_ptr<Payload> &payload) {
        auto token = jwt::create<Traits>()
                .set_issuer(issuer_)
                .set_type("JWS")
                .set_payload_claim("userId", boost::json::value(payload->userId))
                .set_payload_claim("role", boost::json::value(payload->role))
                .sign(jwt::algorithm::hs512{secret_});

        return token;
    }

    std::shared_ptr<JWT::Payload> JWT::readAndVerifyToken(const oatpp::String &token) {
        auto decoded = jwt::decode<Traits>(token);
        verifier_.verify(decoded);

        auto payload = std::make_shared<Payload>();
        // TODO: check if the cast works
        payload->userId = static_cast<uint64_t>(decoded.get_payload_claim("userId").as_int());
        payload->role = static_cast<int32_t>(decoded.get_payload_claim("role").as_int());

        return payload;
    }

}  // namespace server::auth
