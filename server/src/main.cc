#include <oatpp/core/base/Environment.hpp>
#include <oatpp/web/server/api/Endpoint.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp-sqlite/Connection.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp-swagger/Controller.hpp>

#include "component/AppComponent.hh"
#include "controller/AuthController.hh"
#include "controller/OfferController.hh"
#include "controller/UserController.hh"
#include "controller/PlaceController.hh"

int
main() {
    oatpp::base::Environment::init();

    server::component::AppComponent appComponent;

    auto router = appComponent.httpRouterComponent.getObject();

    oatpp::web::server::api::Endpoints endpoints;

    endpoints.append(router->addController(server::controller::AuthController::create_shared())->getEndpoints());
    endpoints.append(router->addController(server::controller::OfferController::createShared())->getEndpoints());
    endpoints.append(router->addController(server::controller::PlaceController::createShared())->getEndpoints());
    endpoints.append(router->addController(server::controller::UserController::createShared())->getEndpoints());


    router->addController(oatpp::swagger::Controller::createShared(endpoints));

    oatpp::network::Server server(
            appComponent.serverConnectionProviderComponent.getObject(),
            appComponent.serverConnectionHandlerComponent.getObject()
    );

    auto const host = appComponent.serverConnectionProviderComponent.getObject()->getProperty("host").toString();
    auto const address = "http://" + (host->find(":") == std::string::npos ? host : "[" + host + "]");

    OATPP_LOGI(
    "Server", "\tListening on %s:%s",
    address->c_str(),
    appComponent.serverConnectionProviderComponent.getObject()->getProperty("port").toString()->c_str()
    )

    server.run();

    OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, databaseConnectionProvider);

    databaseConnectionProvider->stop();

    oatpp::base::Environment::destroy();
    return 0;
}
