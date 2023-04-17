
#include <memory>

#include <iostream>
#include <oatpp-swagger/Controller.hpp>
#include <oatpp/network/Server.hpp>

#include <oatpp/web/server/interceptor/AllowCorsGlobal.hpp>
#include "interceptor/AuthInterceptor.hpp"

#include "component/app_component.hh"
#include "controller/user_controller.hh"
#include "controller/offer_controller.hh"
#include "controller/auth_controller.hh"
#include "controller/parkingSpace_controller.hh"


int
main()
{
    ::oatpp::base::Environment::init();

    ::server::component::app_component app_component;

    auto router = app_component.http_router.getObject();

    ::oatpp::web::server::api::Endpoints endpoints;

    endpoints.append(router->addController(::server::controller::auth_controller::create_shared())->getEndpoints());
    endpoints.append(router->addController(::server::controller::user_controller::create_shared())->getEndpoints());
    //endpoints.append(router->addController(::server::controller::offer_controller::create_shared())->getEndpoints());
    //endpoints.append(router->addController(::server::controller::parkingSpace_controller::create_shared())->getEndpoints());


    router->addController(::oatpp::swagger::Controller::createShared(endpoints));

    oatpp::network::Server server(
        app_component.server_connection_provider.getObject(),
        app_component.server_connection_handler.getObject()
    );


    OATPP_LOGD("Server", "Running on port %s...", app_component.server_connection_provider.getObject()->getProperty("port").toString()->c_str());

    server.run();

    OATPP_COMPONENT(::std::shared_ptr<::oatpp::provider::Provider<::oatpp::sqlite::Connection>>, database_connection_provider);
    database_connection_provider->stop();

    ::oatpp::base::Environment::destroy();
    return 0;
}
