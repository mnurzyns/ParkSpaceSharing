#pragma once

#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/interceptor/AllowCorsGlobal.hpp>

#include "auth/JWT.hh"
#include "DatabaseComponent.hh"
#include "ErrorHandler.hh"
#include "SwaggerComponent.hh"

namespace server::component {

    class AppComponent {

    public:
        server::component::DatabaseComponent databaseComponent;
        server::component::SwaggerComponent swaggerComponent;

        /*
         * JWT component
         * */
        OATPP_CREATE_COMPONENT(std::shared_ptr<auth::JWT>, jwtComponent)([] {
            return std::make_shared<auth::JWT>("secret", "issuer");
        }());

        /*
         * Object mapper component
         * */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapperComponent)([] {
            auto tmp = oatpp::parser::json::mapping::ObjectMapper::createShared();
            tmp->getDeserializer()->getConfig()->allowUnknownFields = false;
            return tmp;
        }());

        /*
         * Server connection provider component
         * */
        OATPP_CREATE_COMPONENT(
                std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                serverConnectionProviderComponent
        )([] {
            return oatpp::network::tcp::server::ConnectionProvider::createShared(
                    {"::1", 8000, oatpp::network::Address::IP_6});
        }());

        /*
         * HTTP Router component
         * */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouterComponent)([] {
            return oatpp::web::server::HttpRouter::createShared();
        }());

        /*
         * Server connection handler component
         * */
        OATPP_CREATE_COMPONENT(
                std::shared_ptr<oatpp::network::ConnectionHandler>,
                serverConnectionHandlerComponent
        )([] {
            OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter);
            OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

            auto tmp = oatpp::web::server::HttpConnectionHandler::createShared(httpRouter);

            tmp->addRequestInterceptor(std::make_shared<oatpp::web::server::interceptor::AllowOptionsGlobal>());

            tmp->addResponseInterceptor(std::make_shared<oatpp::web::server::interceptor::AllowCorsGlobal>());

            tmp->setErrorHandler(std::make_shared<ErrorHandler>(objectMapper));

            return tmp;
        }());

    };

} // namespace server::component
