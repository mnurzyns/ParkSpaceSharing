#include "FullTest.hh"

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

#include "Assert.hh"
#include "ApiClient.hh"
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

oatpp::Object<server::dto::PlaceDto>
placePost(TestEnvironment const& env,
          AuthContext const& auth,
          oatpp::Object<server::dto::PlaceDto> place_dto)
{
    auto response = env.client->place_create_one(auth.token, place_dto);
    testAssert(response->getStatusCode() == 200, assertWrap(response));

    return response->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(
      env.mapper);
}

void
placeDelete(TestEnvironment const& env,
            AuthContext const& auth,
            oatpp::UInt64 const& id)
{
    auto response = env.client->place_delete_one(auth.token, id);
    testAssert(response->getStatusCode() == 200, assertWrap(response));
}

void
placeControllerTests(TestEnvironment const& env, AuthContext const& auth)
{
    auto place_input = server::dto::PlaceDto::createShared();
    place_input->owner_id = auth.token_payload.user_id;
    place_input->address = "West street";
    place_input->latitude = 25.31662036314199;
    place_input->longitude = 51.46711279943629;

    auto place_returned = placePost(env, auth, place_input);

    testAssert(place_returned->id != nullptr &&
               place_input->owner_id == place_returned->owner_id &&
               place_input->address == place_returned->address &&
               place_input->latitude == place_returned->latitude &&
               place_input->longitude == place_returned->longitude);

    placeDelete(env, auth, place_returned->id);
}

oatpp::Object<server::dto::OfferDto>
offerPost(TestEnvironment const& env,
          AuthContext const& auth,
          oatpp::Object<server::dto::OfferDto> const& offer_input)
{
    auto response = env.client->offer_create_one(auth.token, offer_input);
    testAssert(response->getStatusCode() == 200, assertWrap(response));
    auto offer_returned =
      response->readBodyToDto<oatpp::Object<server::dto::OfferDto>>(env.mapper);
    return offer_returned;
}

void
offerDelete(TestEnvironment const& env,
            AuthContext const& auth,
            oatpp::UInt64 const& offer_id)
{
    auto response = env.client->offer_delete_one(auth.token, offer_id);
    testAssert(response->getStatusCode() == 200, assertWrap(response));
}

void
offerControllerTests(TestEnvironment const& env,
                     AuthContext const& auth,
                     oatpp::UInt64 const& place_id)
{
    auto offer_input = server::dto::OfferDto::createShared();
    offer_input->place_id = *place_id;
    offer_input->date_from = 2137;
    offer_input->date_to = 3000;
    offer_input->description = "Cozy parking space";
    offer_input->price = 177013;

    auto offer_returned = offerPost(env, auth, offer_input);
    testAssert(offer_returned->id != nullptr &&
               offer_input->place_id == offer_returned->place_id &&
               offer_input->date_from == offer_returned->date_from &&
               offer_input->date_to == offer_returned->date_to &&
               offer_input->description == offer_returned->description &&
               offer_input->price == offer_returned->price);

    offerDelete(env, auth, offer_returned->id);
}

void
allTests()
{
    TestEnvironment env{};

    userSignUp(env);
    auto auth = userSignIn(env);

    placeControllerTests(env, auth);

    // Place for offer controller
    auto place_input = server::dto::PlaceDto::createShared();
    place_input->owner_id = auth.token_payload.user_id;
    place_input->address = "West street";
    place_input->latitude = 25.31662036314199;
    place_input->longitude = 51.46711279943629;
    auto place_returned = placePost(env, auth, place_input);

    offerControllerTests(env, auth, place_returned->id);
}

} // namespace

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
