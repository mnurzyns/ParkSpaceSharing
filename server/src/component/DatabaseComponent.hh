#pragma once

#include <oatpp-sqlite/ConnectionProvider.hpp>
#include <oatpp-sqlite/Executor.hpp>

#include "database/MainDatabase.hh"

namespace server::component {

    class DatabaseComponent {

    public:
        OATPP_CREATE_COMPONENT(
                std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>,
                databaseConnectionPoolComponent
        )([] {
            auto tmp = std::make_shared<oatpp::sqlite::ConnectionProvider>(MAIN_DATABASE_FILE);
            return oatpp::sqlite::ConnectionPool::createShared(tmp, 10, std::chrono::seconds{5});
        }());

        OATPP_CREATE_COMPONENT(std::shared_ptr<database::MainDatabase>, mainDatabaseComponent)([] {
            OATPP_COMPONENT(
                    std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>,
                    databaseConnectionPool);
            return std::make_shared<server::database::MainDatabase>(
                    std::make_shared<oatpp::sqlite::Executor>(databaseConnectionPool));
        }());
    };

} // namespace server::component

