#pragma once

#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/interceptor/AllowCorsGlobal.hpp>

#include "Config.hh"
#include "DatabaseComponent.hh"
#include "ErrorHandler.hh"
#include "JWT.hh"
#include "SwaggerComponent.hh"

namespace server::component {

using oatpp::data::mapping::ObjectMapper, oatpp::network::ConnectionHandler,
  oatpp::network::ServerConnectionProvider, oatpp::web::server::HttpRouter;

class AppComponent
{

  public:
    server::component::DatabaseComponent database_component;
    server::component::SwaggerComponent swagger_component;

    /*
     * JWT component
     * */
    OATPP_CREATE_COMPONENT(std::shared_ptr<JWT>, jwt_object)
    ([] {
        auto const& config = Config::getInstance();
        return std::make_shared<JWT>(config.jwt_secret, config.jwt_issuer);
    }());

    /*
     * Object mapper component
     * */
    OATPP_CREATE_COMPONENT(std::shared_ptr<ObjectMapper>, object_mapper)
    ([] {
        auto tmp = oatpp::parser::json::mapping::ObjectMapper::createShared();
        tmp->getDeserializer()->getConfig()->allowUnknownFields = false;
        return tmp;
    }());

    /*
     * Server connection provider component
     * */
    OATPP_CREATE_COMPONENT(std::shared_ptr<ServerConnectionProvider>,
                           server_connection_provider)
    ([] {
        auto const& config = Config::getInstance();
        return oatpp::network::tcp::server::ConnectionProvider::createShared(
          { config.bind, config.port }, true);
    }());

    /*
     * HTTP Router component
     * */
    OATPP_CREATE_COMPONENT(std::shared_ptr<HttpRouter>, http_router)
    ([] { return HttpRouter::createShared(); }());

    /*
     * Server connection handler component
     * */
    OATPP_CREATE_COMPONENT(std::shared_ptr<ConnectionHandler>,
                           connection_handler)
    ([] {
        const OATPP_COMPONENT(std::shared_ptr<HttpRouter>,
                              http_router_component);
        const OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                              object_mapper_component);

        auto tmp = oatpp::web::server::HttpConnectionHandler::createShared(
          http_router_component);

        tmp->addRequestInterceptor(
          std::make_shared<
            oatpp::web::server::interceptor::AllowOptionsGlobal>());

        tmp->addResponseInterceptor(
          std::make_shared<oatpp::web::server::interceptor::AllowCorsGlobal>());

        tmp->setErrorHandler(
          std::make_shared<ErrorHandler>(object_mapper_component));

        return tmp;
    }());
};

} // namespace server::component
