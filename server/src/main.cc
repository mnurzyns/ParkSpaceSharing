#include <memory>

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/core/provider/Provider.hpp>
#include <oatpp/network/ConnectionHandler.hpp>
#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/web/server/api/Endpoint.hpp>
#include <oatpp-swagger/Controller.hpp>

#include "component/app_component.hh"
#include "controller/user_controller.hh"
#include "controller/offer_controller.hh"
#include "controller/auth_controller.hh"

int
main()
{
    ::oatpp::base::Environment::init();

    ::server::component::app_component const app_component{};

    OATPP_COMPONENT(::std::shared_ptr<::oatpp::web::server::HttpRouter>, router);

    ::oatpp::web::server::api::Endpoints endpoints{};
    endpoints.append(router->addController(::server::controller::auth_controller::create_shared())->getEndpoints());
    endpoints.append(router->addController(::server::controller::user_controller::create_shared())->getEndpoints());
    endpoints.append(router->addController(::server::controller::offer_controller::create_shared())->getEndpoints());

    router->addController(::oatpp::swagger::Controller::createShared(endpoints));

    OATPP_COMPONENT(::std::shared_ptr<::oatpp::network::ServerConnectionProvider>, server_connection_provider);
    OATPP_COMPONENT(::std::shared_ptr<::oatpp::network::ConnectionHandler>, connection_handler);

    ::oatpp::network::Server server{server_connection_provider, connection_handler};
    OATPP_LOGD("Server", "Server running on port %s", server_connection_provider->getProperty("port").toString()->c_str());

    server.run();

    OATPP_COMPONENT(::std::shared_ptr<::oatpp::provider::Provider<::oatpp::sqlite::Connection>>, database_connection_provider);
    database_connection_provider->stop();

    ::oatpp::base::Environment::destroy();
    return 0;
}