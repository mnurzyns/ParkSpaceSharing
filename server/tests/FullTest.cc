#include "FullTest.hh"
#include "dto/PlaceDto.hh"

#define CONFIG_ALLOW_MUTATION
#include "Config.hh"

#include "TokenUtils.hh"
#include "component/AppComponent.hh"
#include "controller/AuthController.hh"
#include "controller/OfferController.hh"
#include "controller/PlaceController.hh"
#include "controller/UserController.hh"

#include <oatpp-test/UnitTest.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/client/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/client/ApiClient.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>
#include <oatpp/web/protocol/http/incoming/Response.hpp>

#include "ApiClient.hh"
#include "Assert.hh"
#include "Context.hh"

namespace tests {

void
userSignUp(TestEnvironment const& env)
{
    auto signup_dto = server::dto::SignUpDto::createShared();
    signup_dto->email = "string";
    signup_dto->username = "string";
    signup_dto->password = "string";

    auto response = env.client->signup(signup_dto);
    testAssert(response->getStatusCode() == 201, assertWrap(response));
}

AuthContext
userSignIn(TestEnvironment const& env)
{
    auto signin_dto = server::dto::SignInDto::createShared();
    signin_dto->login = "string";
    signin_dto->password = "string";

    auto response = env.client->signin(signin_dto);
    testAssert(response->getStatusCode() == 200, assertWrap(response));

    auto auth_dto =
      response->readBodyToDto<oatpp::Object<server::dto::AuthDto>>(env.mapper);
    testAssert(auth_dto != nullptr);
    testAssert(auth_dto->token != nullptr && !auth_dto->token->empty());

    auto token_payload =
      server::TokenUtils::readToken(jwt::decode(auth_dto->token));

    return AuthContext{ .token = auth_dto->token,
                        .token_payload = token_payload };
}

void
placePostTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[PlaceController][POST][200]", "Valid request");
    deferFailure([&] {
        auto dto = server::dto::PlaceDto::createShared();
        dto->owner_id = auth.token_payload.user_id;
        dto->address = "West street";
        dto->latitude = 25.31662036314199;
        dto->longitude = 51.46711279943629;

        auto res = env.client->placePost(auth.token, dto);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(env.mapper);
        testAssert(returned->id != nullptr &&
                   returned->owner_id == dto->owner_id &&
                   returned->address == dto->address &&
                   returned->latitude == dto->latitude &&
                   returned->longitude == dto->longitude);
    });

    OATPP_LOGD("[PlaceController][POST][400]", "Bad request");
    deferFailure([&] {
        auto dto = server::dto::PlaceDto::createShared();
        dto->owner_id = auth.token_payload.user_id;
        dto->address = nullptr;
        dto->latitude = 25.31662036314199;
        dto->longitude = 51.46711279943629;

        auto res = env.client->placePost(auth.token, dto);
        testAssert(res->getStatusCode() == 400, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][POST][401]", "Unauthorized");
    deferFailure([&] {
        auto dto = server::dto::PlaceDto::createShared();
        dto->owner_id = auth.token_payload.user_id;
        dto->address = "West street";
        dto->latitude = 25.31662036314199;
        dto->longitude = 51.46711279943629;

        auto res = env.client->placePost("", dto);
        testAssert(res->getStatusCode() == 401, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][POST][403]", "Forbidden");
    deferFailure([&] {
        auto dto = server::dto::PlaceDto::createShared();
        dto->owner_id = 1; // Created by migrations
        dto->address = "West street";
        dto->latitude = 25.31662036314199;
        dto->longitude = 51.46711279943629;

        auto res = env.client->placePost(auth.token, dto);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][POST][409]", "Conflict");
    deferFailure([&] {
        auto dto = server::dto::PlaceDto::createShared();
        dto->id = 1;
        dto->owner_id = auth.token_payload.user_id;
        dto->address = "West street";
        dto->latitude = 25.31662036314199;
        dto->longitude = 51.46711279943629;

        auto res = env.client->placePost(auth.token, dto);
        testAssert(res->getStatusCode() == 409, assertWrap(res));
    });
}

/*
  Requires working POST endpoint.
 */
oatpp::Object<server::dto::PlaceDto>
createDummyPlace(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[PlaceController]", "Create dummy place");
    auto dto = server::dto::PlaceDto::createShared();
    dto->owner_id = auth.token_payload.user_id;
    dto->address = "Dummy place";
    dto->latitude = 25.31662036314199;
    dto->longitude = 51.46711279943629;

    auto res = env.client->placePost(auth.token, dto);
    testAssert(res->getStatusCode() == 200, assertWrap(res));
    return res->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(env.mapper);
}

void
placeGetByIdTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[PlaceController][GET][200]", "Valid request");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);

        auto res = env.client->placeGetById(dto->id);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(env.mapper);
        testAssert(returned->id == dto->id &&
                   returned->owner_id == dto->owner_id &&
                   returned->address == dto->address &&
                   returned->latitude == dto->latitude &&
                   returned->longitude == dto->longitude);
    });

    OATPP_LOGD("[PlaceController][GET][404]", "Not found");
    deferFailure([&] {
        auto res = env.client->placeGetById(177013);
        testAssert(res->getStatusCode() == 404, assertWrap(res));
    });
}

void
placePatchTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[PlaceController][PATCH][200]", "Valid request");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        auto patch = server::dto::PlaceDto::createShared();
        patch->address = "East street";

        auto res = env.client->placePatch(auth.token, dto->id, patch);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(env.mapper);
        testAssert(returned->id == dto->id &&
                   returned->owner_id == dto->owner_id &&
                   returned->address == "East street" &&
                   returned->latitude == dto->latitude &&
                   returned->longitude == dto->longitude);
    });

    OATPP_LOGD("[PlaceController][PATCH][401]", "Unauthorized");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        auto patch = server::dto::PlaceDto::createShared();
        patch->address = "East street";

        auto res = env.client->placePatch("", dto->id, patch);
        testAssert(res->getStatusCode() == 401, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][PATCH][403]", "Forbidden - id");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        auto patch = server::dto::PlaceDto::createShared();
        // Place id=1 created by migrations.
        patch->id = 1;

        auto res = env.client->placePatch(auth.token, dto->id, patch);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][PATCH][403]", "Forbidden - owner_id");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        auto patch = server::dto::PlaceDto::createShared();
        // User id=1 created by migrations.
        patch->id = dto->id;
        patch->owner_id = 1;

        auto res = env.client->placePatch(auth.token, dto->id, patch);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][PATCH][404]", "Not found");
    deferFailure([&] {
        auto patch = server::dto::PlaceDto::createShared();

        auto res = env.client->placePatch(auth.token, 177013, patch);
        testAssert(res->getStatusCode() == 404, assertWrap(res));
    });
}

void
placePutTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[PlaceController][PUT][200]", "Valid request");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        dto->address = "East street";
        dto->latitude = 123.123;
        dto->longitude = 321.321;

        auto res = env.client->placePut(auth.token, dto);
        testAssert(res->getStatusCode() == 200, assertWrap(res));

        auto returned =
          res->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(env.mapper);
        testAssert(returned->id == dto->id &&
                   returned->owner_id == dto->owner_id &&
                   returned->address == dto->address &&
                   returned->latitude == dto->latitude &&
                   returned->longitude == dto->longitude);
    });

    OATPP_LOGD("[PlaceController][PUT][400]", "Bad request");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        dto->latitude = nullptr;

        auto res = env.client->placePut(auth.token, dto);
        testAssert(res->getStatusCode() == 400, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][PUT][401]", "Unauthorized");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);

        auto res = env.client->placePut("", dto);
        testAssert(res->getStatusCode() == 401, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][PUT][403]", "Forbidden - id");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        // Place id=1 created by migrations.
        dto->id = 1;

        auto res = env.client->placePut(auth.token, dto);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][PUT][403]", "Forbidden - owner_id");
    deferFailure([&] {
        auto dto = createDummyPlace(env, auth);
        // User id=1 created by migrations.
        dto->owner_id = 1;

        auto res = env.client->placePut(auth.token, dto);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });
}

void
placeDeleteTest(TestEnvironment const& env, AuthContext const& auth)
{
    OATPP_LOGD("[PlaceController][DELETE][200]", "Valid request");
    deferFailure([&] {
        auto place = createDummyPlace(env, auth);

        auto res = env.client->placeDelete(auth.token, place->id);
        testAssert(res->getStatusCode() == 200, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][DELETE][401]", "Unauthorized");
    deferFailure([&] {
        auto place = createDummyPlace(env, auth);

        auto res = env.client->placeDelete("", place->id);
        testAssert(res->getStatusCode() == 401, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][DELETE][403]", "Unauthorized");
    deferFailure([&] {
        // Place id=1 created by migrations.
        auto res = env.client->placeDelete(auth.token, 1);
        testAssert(res->getStatusCode() == 403, assertWrap(res));
    });

    OATPP_LOGD("[PlaceController][DELETE][404]", "Not found");
    deferFailure([&] {
        auto res = env.client->placeDelete(auth.token, 177013);
        testAssert(res->getStatusCode() == 404, assertWrap(res));
    });
}

void
allTests()
{
    TestEnvironment env{};

    userSignUp(env);
    auto auth = userSignIn(env);

    placePostTest(env, auth);
    placeGetByIdTest(env, auth);
    placePatchTest(env, auth);
    placePutTest(env, auth);
    placeDeleteTest(env, auth);

    assertDeferredFailures();
}

} // namespace tests

void
FullTest::onRun()
{
    auto& conf = server::Config::getInstanceMut();
    conf.database_path = ":memory:";

    server::component::AppComponent component{};
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                    conn_provider);
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
                    conn_handler);
    OATPP_COMPONENT(
      std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>,
      db_conn_provider);

    router->addController(server::controller::AuthController::createShared());
    router->addController(server::controller::UserController::createShared());
    router->addController(server::controller::PlaceController::createShared());
    router->addController(server::controller::OfferController::createShared());

    oatpp::network::Server server(conn_provider, conn_handler);

    std::thread server_thread{ [&server] { server.run(); } };

    tests::allTests();

    server.stop();
    db_conn_provider->stop();
    conn_handler->stop();
    conn_provider->stop();

    server_thread.join();
}
