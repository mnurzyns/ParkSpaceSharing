#include <oatpp/core/base/Environment.hpp>
#include <oatpp/web/server/api/Endpoint.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp-sqlite/Connection.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp-swagger/Controller.hpp>

#include "component/AppComponent.hh"
#include "controller/TestController.hh"
#include "controller/AuthController.hh"

int
main()
{
    oatpp::base::Environment::init();

    server::component::AppComponent app_component;

    auto router = app_component.httpRouterComponent.getObject();

    oatpp::web::server::api::Endpoints endpoints;

    endpoints.append(router->addController(server::controller::TestController::create_shared())->getEndpoints());
    endpoints.append(router->addController(server::controller::AuthController::create_shared())->getEndpoints());


    router->addController(oatpp::swagger::Controller::createShared(endpoints));

    oatpp::network::Server server(
        app_component.serverConnectionProviderComponent.getObject(),
        app_component.serverConnectionHandlerComponent.getObject()
    );


    OATPP_LOGD("Server", "Running on port %s...", app_component.serverConnectionProviderComponent.getObject()->getProperty("port").toString()->c_str());

    server.run();

    OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, databaseConnectionProvider);

    databaseConnectionProvider->stop();

    oatpp::base::Environment::destroy();
    return 0;
}
