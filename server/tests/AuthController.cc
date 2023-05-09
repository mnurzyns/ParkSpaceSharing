#include "AuthController.hh"

#include "Assert.hh"
#include "TokenPayload.hh"
#include "TokenUtils.hh"
#include "dto/AuthDto.hh"
#include "dto/SignInDto.hh"
#include "dto/SignUpDto.hh"
#include <jwt-cpp/jwt.h>
#include <thread>

namespace tests {

static std::atomic<unsigned> g_phone_last_i = 100;


// TODO(Piotr Stefańsk): Add tests targeting phone numbers.
void
signupPostTest(TestEnvironment const& env)
{
    OATPP_LOGD("[AuthController][signup][200]", "Valid request");
    deferFailure([&] {
        auto dto = server::dto::SignUpDto::createShared();

        // Add line number to username and email to avoid collision.
        dto->username = "jan" + std::to_string(__LINE__);
        dto->email = "jan@jan.jan" + std::to_string(__LINE__);
        dto->phone = "+48 123 123 " + std::to_string(g_phone_last_i++);
        dto->password = "jan";

        auto res = env.client->signup(dto);
        testAssert(res->getStatusCode() == 201, assertWrap(res));
    });

    OATPP_LOGD("[AuthController][signup][400]", "Bad request - missing field");
    deferFailure([&] {
        auto dto = server::dto::SignUpDto::createShared();

        // Add line number to username and email to avoid collision.
        dto->username = "jan" + std::to_string(__LINE__);
        dto->email = "jan@jan.jan" + std::to_string(__LINE__);
        dto->phone = "+48 123 123 " + std::to_string(g_phone_last_i++);
        dto->password = nullptr;

        auto res = env.client->signup(dto);
        testAssert(res->getStatusCode() == 400, assertWrap(res));
    });

    OATPP_LOGD("[AuthController][signup][400]", "Bad request - invalid email");
    deferFailure([&] {
        auto dto = server::dto::SignUpDto::createShared();

        // Add line number to username and email to avoid collision.
        dto->username = "jan" + std::to_string(__LINE__);
        dto->email = std::to_string(__LINE__);
        dto->phone = "+48 123 123 " + std::to_string(g_phone_last_i++);
        dto->password = "jan";

        auto res = env.client->signup(dto);
        testAssert(res->getStatusCode() == 400, assertWrap(res));
    });

    OATPP_LOGD("[AuthController][signup][409]", "Conflict");
    deferFailure([&] {
        auto dto = server::dto::SignUpDto::createShared();

        // Add line number to username and email to avoid collision.
        dto->username = "jan" + std::to_string(__LINE__);
        dto->email = "jan@jan.jan" + std::to_string(__LINE__);
        dto->phone = "+48 123 123 " + std::to_string(g_phone_last_i++);
        dto->password = "jan";

        auto res1 = env.client->signup(dto);
        testAssert(res1->getStatusCode() == 201, assertWrap(res1));

        auto res2 = env.client->signup(dto);
        testAssert(res2->getStatusCode() == 409, assertWrap(res2));
    });
}

void
signinPostTest(TestEnvironment const& env)
{
    OATPP_LOGD("[AuthController][signin][200]", "Valid request - username login");
    deferFailure([&] {
        auto signup = signupDummyUser(env);
        auto dto = server::dto::SignInDto::createShared();
        dto->login = signup->username;
        dto->password = signup->password;

        auto res = env.client->signin(dto);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto auth =
          res->readBodyToDto<oatpp::Object<server::dto::AuthDto>>(env.mapper);
        server::TokenUtils::readToken(
          jwt::decoded_jwt<server::JsonTraits>(auth->token));
    });

    OATPP_LOGD("[AuthController][signin][200]", "Valid request - email login");
    deferFailure([&] {
        auto signup = signupDummyUser(env);
        auto dto = server::dto::SignInDto::createShared();
        dto->login = signup->email;
        dto->password = signup->password;

        auto res = env.client->signin(dto);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto auth =
          res->readBodyToDto<oatpp::Object<server::dto::AuthDto>>(env.mapper);
        server::TokenUtils::readToken(
          jwt::decoded_jwt<server::JsonTraits>(auth->token));
    });

    OATPP_LOGD("[AuthController][signin][400]", "Bad request");
    deferFailure([&] {
        auto dto = server::dto::SignInDto::createShared();
        dto->login = "joe";
        dto->password = nullptr;

        auto res = env.client->signin(dto);
        testAssert(res->getStatusCode() == 400, assertWrap(res));
    });

    OATPP_LOGD("[AuthController][signin][401]", "Unauthorized");
    deferFailure([&] {
        auto signup = signupDummyUser(env);
        auto dto = server::dto::SignInDto::createShared();
        dto->login = signup->email;
        dto->password = signup->password + "invalidpassword";

        auto res = env.client->signin(dto);
        testAssert(res->getStatusCode() == 401, assertWrap(res));
    });

    OATPP_LOGD("[AuthController][signin][404]", "Not found");
    deferFailure([&] {
        auto dto = server::dto::SignInDto::createShared();
        dto->login = "177013";
        dto->password = "haslomaslo";

        auto res = env.client->signin(dto);
        testAssert(res->getStatusCode() == 404, assertWrap(res));
    });
}

oatpp::Object<server::dto::SignUpDto>
signupDummyUser(TestEnvironment const& env)
{
    auto dto = server::dto::SignUpDto::createShared();

    // Add line number and offset to username and email to avoid collision.
    static int offset = 0;
    dto->username = "jan" + std::to_string(__LINE__ + offset++);
    dto->email = "jan@jan.jan" + std::to_string(__LINE__ + offset++);
    dto->password = "jan";
    dto->phone = "+48 123 123 " + std::to_string(g_phone_last_i++);

    auto res = env.client->signup(dto);
    testAssert(res->getStatusCode() == 201, assertWrap(res));

    return dto;
}

AuthContext
signinAsDummyUser(TestEnvironment const& env)
{
    auto signup = signupDummyUser(env);
    auto dto = server::dto::SignInDto::createShared();
    dto->login = signup->username;
    dto->password = signup->password;

    auto res = env.client->signin(dto);
    testAssert(res->getStatusCode() == 200, assertWrap(res));

    auto auth =
      res->readBodyToDto<oatpp::Object<server::dto::AuthDto>>(env.mapper);
    return AuthContext{ .token = auth->token,
                        .token_payload = server::TokenUtils::readToken(
                          jwt::decoded_jwt<server::JsonTraits>(auth->token)) };
}

} // namespace tests
