#pragma once

#include <chrono>
#include <memory>
#include <oatpp-sqlite/Connection.hpp>
#include <oatpp-sqlite/ConnectionProvider.hpp>
#include <oatpp-sqlite/Executor.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/core/provider/Provider.hpp>

#include "database/parking_space_sharing.hh"

namespace server::component
{

class database
{
public:
    OATPP_CREATE_COMPONENT(::std::shared_ptr<::oatpp::provider::Provider<::oatpp::sqlite::Connection>>, database_connection_provider)([]{
        auto connection_provider = ::std::make_shared<::oatpp::sqlite::ConnectionProvider>(PSS_DATABASE_FILE);
        return ::oatpp::sqlite::ConnectionPool::createShared(connection_provider, 10, std::chrono::seconds{5});
    }());

    OATPP_CREATE_COMPONENT(::std::shared_ptr<::server::database::parking_space_sharing>, database_parking_space_sharing)([]{
        OATPP_COMPONENT(::std::shared_ptr<::oatpp::provider::Provider<::oatpp::sqlite::Connection>>, connection_provider);
        return ::std::make_shared<::server::database::parking_space_sharing>(::std::make_shared<::oatpp::sqlite::Executor>(connection_provider));
    }());
};

} // namespace server::component

