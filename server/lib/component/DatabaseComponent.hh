#pragma once

#include <oatpp-sqlite/ConnectionProvider.hpp>
#include <oatpp-sqlite/Executor.hpp>

#include "Config.hh"
#include "database/MainDatabase.hh"

namespace server::component {

class DatabaseComponent
{

  public:
    OATPP_CREATE_COMPONENT(
      std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>,
      database_connection_provider)
    ([] {
        auto const& database_path = server::Config::getInstance().database_path;
        OATPP_LOGI("Database", "\tReading database from \"%s\"", database_path.c_str());
        auto tmp =
          std::make_shared<oatpp::sqlite::ConnectionProvider>(database_path);
        return oatpp::sqlite::ConnectionPool::createShared(
          tmp, 10, std::chrono::seconds{ 5 });
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<database::MainDatabase>,
                           main_database)
    ([] {
        const OATPP_COMPONENT(
          std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>,
          database_connection_provider_component);
        return std::make_shared<server::database::MainDatabase>(
          std::make_shared<oatpp::sqlite::Executor>(
            database_connection_provider_component));
    }());
};

} // namespace server::component
