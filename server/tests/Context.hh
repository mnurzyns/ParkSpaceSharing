#pragma once

#include <oatpp/network/tcp/client/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/client/HttpRequestExecutor.hpp>

#include "TokenPayload.hh"

#include <memory>

#include "ApiClient.hh"

namespace tests {

struct TestEnvironment
{
    TestEnvironment()
      : conn_provider(
          oatpp::network::tcp::client::ConnectionProvider::createShared(
            { "localhost", 8000 }))
      , executor(
          oatpp::web::client::HttpRequestExecutor::createShared(conn_provider))
      , mapper(oatpp::parser::json::mapping::ObjectMapper::createShared())
      , client(ApiClient::createShared(executor, mapper))
    {
    }

    std::shared_ptr<oatpp::network::tcp::client::ConnectionProvider>
      conn_provider;
    std::shared_ptr<oatpp::web::client::HttpRequestExecutor> executor;
    std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> mapper;
    std::shared_ptr<ApiClient> client;
};

struct AuthContext
{
    std::string token;
    server::TokenPayload token_payload;
};

} // namespace tests
