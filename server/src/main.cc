#include <oatpp-sqlite/Connection.hpp>
#include <oatpp-swagger/Controller.hpp>
#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/web/server/api/Endpoint.hpp>

#include "component/AppComponent.hh"
#include "controller/AuthController.hh"
#include "controller/OfferController.hh"
#include "controller/PlaceController.hh"
#include "controller/UserController.hh"

using namespace server::controller; // NOLINT

int
main()
{
    oatpp::base::Environment::init();

    server::component::AppComponent app_component;

    const OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>,
                          http_router);

    oatpp::web::server::api::Endpoints endpoints;

    endpoints.append(http_router->addController(AuthController::createShared())
                       ->getEndpoints());
    endpoints.append(http_router->addController(OfferController::createShared())
                       ->getEndpoints());
    endpoints.append(http_router->addController(PlaceController::createShared())
                       ->getEndpoints());
    endpoints.append(http_router->addController(UserController::createShared())
                       ->getEndpoints());
    http_router->addController(
      oatpp::swagger::Controller::createShared(endpoints));

    oatpp::network::Server server(
      app_component.server_connection_provider.getObject(),
      app_component.connection_handler.getObject());

    auto const host = app_component.server_connection_provider.getObject()
                        ->getProperty("host")
                        .toString();
    auto const address = // Add square brackets for IPv6 addresses
      "http://" +
      (host->find(":") == std::string::npos ? host : "[" + host + "]");

    OATPP_LOGI("Server",
               "\tListening on %s:%s",
               address->c_str(),
               app_component.server_connection_provider.getObject()
                 ->getProperty("port")
                 .toString()
                 ->c_str())

    server.run();

    OATPP_COMPONENT(const std::shared_ptr<
                      oatpp::provider::Provider<oatpp::sqlite::Connection>>,
                    database_connection_provider);

    database_connection_provider->stop();
    oatpp::base::Environment::destroy();
    return 0;
}
