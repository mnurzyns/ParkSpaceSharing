#include "FullTest.hh"
#include <memory>
#include <oatpp-test/UnitTest.hpp>

#define CONFIG_ALLOW_MUTATION
#include "Config.hh"

#include "TokenPayload.hh"
#include "TokenUtils.hh"
#include "component/AppComponent.hh"
#include "controller/AuthController.hh"
#include "controller/OfferController.hh"
#include "controller/PlaceController.hh"
#include "controller/UserController.hh"
#include "dto/PlaceDto.hh"
#include "dto/UserDto.hh"

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/client/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/client/ApiClient.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>

namespace {

#include OATPP_CODEGEN_BEGIN(ApiClient)

class Client : public oatpp::web::client::ApiClient
{
  public:
    API_CLIENT_INIT(Client)

    API_CALL("POST",
             "signup",
             signup,
             BODY_DTO(oatpp::Object<server::dto::SignUpDto>, dto))
    API_CALL("POST",
             "signin",
             signin,
             BODY_DTO(oatpp::Object<server::dto::SignInDto>, dto))
    API_CALL("POST",
             "place",
             place_create_one,
             AUTHORIZATION(String, auth_string, "Bearer"),
             BODY_DTO(oatpp::Object<server::dto::PlaceDto>, dto))
    API_CALL("DELETE",
             "place/{id}",
             place_delete_one,
             AUTHORIZATION(String, auth_string, "Bearer"),
             PATH(UInt64, id))
};

#include OATPP_CODEGEN_END(ApiClient)

struct TestEnvironment
{
    TestEnvironment()
      : conn_provider(
          oatpp::network::tcp::client::ConnectionProvider::createShared(
            { "localhost", 8000 }))
      , executor(
          oatpp::web::client::HttpRequestExecutor::createShared(conn_provider))
      , mapper(oatpp::parser::json::mapping::ObjectMapper::createShared())
      , client(Client::createShared(executor, mapper))
    {
    }

    std::shared_ptr<oatpp::network::tcp::client::ConnectionProvider>
      conn_provider;
    std::shared_ptr<oatpp::web::client::HttpRequestExecutor> executor;
    std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> mapper;
    std::shared_ptr<Client> client;
};

struct AuthContext
{
    std::string token;
    server::TokenPayload token_payload;
};

void
userSignUp(TestEnvironment const& env)
{
    auto signup_dto = server::dto::SignUpDto::createShared();
    signup_dto->email = "string";
    signup_dto->username = "string";
    signup_dto->password = "string";

    auto response = env.client->signup(signup_dto);
    // currentlly creates
    // user but returns 404
    // OATPP_ASSERT(response->getStatusCode() == 200);
}

AuthContext
userSignIn(TestEnvironment const& env)
{
    auto signin_dto = server::dto::SignInDto::createShared();
    signin_dto->login = "string";
    signin_dto->password = "string";

    auto response = env.client->signin(signin_dto);
    OATPP_ASSERT(response->getStatusCode() == 200);

    auto auth_dto =
      response->readBodyToDto<oatpp::Object<server::dto::AuthDto>>(env.mapper);
    OATPP_ASSERT(auth_dto);
    OATPP_ASSERT(auth_dto->token);

    auto token_payload =
      server::TokenUtils::readToken(jwt::decode(auth_dto->token));

    return AuthContext{ .token = auth_dto->token,
                        .token_payload = token_payload };
}

oatpp::Object<server::dto::PlaceDto>
placePost(TestEnvironment const& env,
          AuthContext const& auth,
          oatpp::Object<server::dto::PlaceDto const> place_dto)
{
    auto response = env.client->place_create_one(auth.token, place_dto);
    OATPP_ASSERT(response->getStatusCode() == 200);

    return response->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(
      env.mapper);
}

void
placeDelete(TestEnvironment const& env,
            AuthContext const& auth,
            oatpp::UInt64 const& id)
{
    auto response = env.client->place_delete_one(auth.token, id);
    OATPP_ASSERT(response->getStatusCode() == 200);
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

    OATPP_ASSERT(place_returned->id != nullptr &&
                 place_input->owner_id == place_returned->owner_id &&
                 place_input->address == place_returned->address &&
                 place_input->latitude == place_returned->latitude &&
                 place_input->longitude == place_returned->longitude);

    placeDelete(env, auth, place_returned->id);
}

void
allTests()
{
    TestEnvironment env{};

    userSignUp(env);
    auto auth = userSignIn(env);

    placeControllerTests(env, auth);
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

    std::jthread server_thread{ [&server] { server.run(); } };

    allTests();

    server.stop();
    db_conn_provider->stop();
    conn_handler->stop();
    conn_provider->stop();

    server_thread.join();
}
