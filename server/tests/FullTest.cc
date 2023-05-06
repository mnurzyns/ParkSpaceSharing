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

#include "AuthController.hh"
#include "PlaceController.hh"
#include "OfferController.hh"

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
allTests()
{
    TestEnvironment env{};

    userSignUp(env);
    auto auth = userSignIn(env);

    signupPostTest(env);
    signinPostTest(env);

    placePostTest(env, auth);
    placeGetByIdTest(env, auth);
    placePatchTest(env, auth);
    placePutTest(env, auth);
    placeDeleteTest(env, auth);

    offerPostTest(env, auth);
    offerGetByIdTest(env, auth);
    offerPatchTest(env, auth);
    offerPutTest(env, auth);
    offerDeleteTest(env, auth);

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
