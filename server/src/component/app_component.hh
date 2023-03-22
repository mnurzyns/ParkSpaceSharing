#pragma once

#include <memory>

#include <oatpp/core/data/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/interceptor/AllowCorsGlobal.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/Address.hpp>
#include <oatpp/network/ConnectionHandler.hpp>
#include <oatpp/network/ConnectionProvider.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>

#include "error_handler.hh"

#include "interceptor/AuthInterceptor.hpp"

#include "component/database_component.hh"
#include "component/swagger_component.hh"

namespace server::component
{

class app_component
{
public:
    ::server::component::database_component database_component;
    ::server::component::swagger_component swagger_component;

    OATPP_CREATE_COMPONENT(::std::shared_ptr<JWT>, jwt_)([]{
    return ::std::make_shared<JWT>("secret", "issuer");
    }());

    OATPP_CREATE_COMPONENT(::std::shared_ptr<::oatpp::data::mapping::ObjectMapper>, api_object_mapper)([]{
        auto object_mapper = ::oatpp::parser::json::mapping::ObjectMapper::createShared();
        object_mapper->getDeserializer()->getConfig()->allowUnknownFields = false;

        return object_mapper;
    }());

    OATPP_CREATE_COMPONENT(::std::shared_ptr<::oatpp::network::ServerConnectionProvider>, server_connection_provider)([]{
        return ::oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", 8000, ::oatpp::network::Address::IP_4});
    }());

    OATPP_CREATE_COMPONENT(::std::shared_ptr<::oatpp::web::server::HttpRouter>, http_router)([]{
        return ::oatpp::web::server::HttpRouter::createShared();
    }());

    OATPP_CREATE_COMPONENT(::std::shared_ptr<::oatpp::network::ConnectionHandler>, server_connection_handler)([]{

        OATPP_COMPONENT(::std::shared_ptr<JWT>, jwt_m);
        OATPP_COMPONENT(::std::shared_ptr<::oatpp::web::server::HttpRouter>, router);
        OATPP_COMPONENT(::std::shared_ptr<::oatpp::data::mapping::ObjectMapper>, object_mapper);

        auto connection_handler = ::oatpp::web::server::HttpConnectionHandler::createShared(router);
        connection_handler->setErrorHandler(::std::make_shared<::server::error_handler>(object_mapper));

        connection_handler->addRequestInterceptor(::std::make_shared<oatpp::web::server::interceptor::AllowOptionsGlobal>());
        connection_handler->addRequestInterceptor(::std::make_shared<AuthInterceptor>(jwt_m));
        connection_handler->addResponseInterceptor(::std::make_shared<oatpp::web::server::interceptor::AllowCorsGlobal>());

        return connection_handler;
    }());

};

} // namespace server::component
