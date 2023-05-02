#include "FullTest.hh"

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

void
userPostTest()
{
    auto conn_provider =
      oatpp::network::tcp::client::ConnectionProvider::createShared(
        { "localhost", 8000 });
    auto executor =
      oatpp::web::client::HttpRequestExecutor::createShared(conn_provider);
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto client = Client::createShared(executor, mapper);

    oatpp::Object<server::dto::AuthDto> auth{};
    {
        auto signup_dto = server::dto::SignUpDto::createShared();
        signup_dto->email = "string";
        signup_dto->username = "string";
        signup_dto->password = "string";

        auto response = client->signup(signup_dto);
        // OATPP_ASSERT(response->getStatusCode() == 200);
        OATPP_LOGD("TEST", "Created user");
    }

    // Log in
    {
        auto signin_dto = server::dto::SignInDto::createShared();
        signin_dto->login = "string";
        signin_dto->password = "string";

        auto response = client->signin(signin_dto);
        OATPP_ASSERT(response->getStatusCode() == 200);
        OATPP_LOGD("TEST", "Loged in.");

        auth =
          response->readBodyToDto<oatpp::Object<server::dto::AuthDto>>(mapper);
        OATPP_ASSERT(auth);
        OATPP_ASSERT(auth->token);
        OATPP_LOGD("TEST", "Auth token=\"%s\"", auth->token->c_str());
    }
    auto token_payload =
      server::TokenUtils::readToken(jwt::decode(auth->token));

    // Create one place
    oatpp::Object<server::dto::PlaceDto> place{};
    {
        auto place_dto = server::dto::PlaceDto::createShared();
        place_dto->owner_id = token_payload.user_id;
        place_dto->address = "West street";
        place_dto->latitude = 25.31662036314199;
        place_dto->longitude = 51.46711279943629;

        auto response = client->place_create_one(auth->token, place_dto);
        OATPP_ASSERT(response->getStatusCode() == 200);

        place =
          response->readBodyToDto<oatpp::Object<server::dto::PlaceDto>>(mapper);
        OATPP_LOGD("TEST", "Created place: %lu\n", *place->id);
    }

    // Remove previously created place
    {
        auto response = client->place_delete_one(auth->token, place->id);
        OATPP_ASSERT(response->getStatusCode() == 200);
        OATPP_LOGD("TEST", "Deleted place: %lu\n", *place->id);
    }
}

} // namespace

void
FullTest::onRun()
{
    // TODO(Piotr Stefański): Make sure the database is always in the same
    // state.
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

    userPostTest();

    server.stop();
    db_conn_provider->stop();
    conn_handler->stop();
    conn_provider->stop();

    server_thread.join();
}
