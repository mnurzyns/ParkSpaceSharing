#pragma once

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <oatpp/core/base/Environment.hpp>
#include <string>
#include <vector>

#include <toml.hpp>

namespace server {

// When changing value types of members
// also update template argument of fromToml().
struct Config
{
    std::string bind;
    std::uint16_t port;

    std::string database_path;

    std::string jwt_secret;
    std::string jwt_issuer;
    std::uint64_t jwt_expire_after;

    std::string swagger_res_path;

    static Config const&
    getInstance();
};

} // namespace server

namespace toml {

using server::Config;
using std::chrono::duration_cast, std::chrono::days, std::chrono::milliseconds;

template<>
struct [[maybe_unused]] from<Config>
{
    [[maybe_unused]] static Config
    from_toml(toml::value const& val)
    {
        auto const& server = toml::find_or(val, "server", toml::value{});
        auto const& jwt = toml::find_or(server, "jwt", toml::value{});
        auto const& swagger = toml::find_or(server, "swagger", toml::value{});

        return Config{
            .bind = toml::find_or<std::string>(server, "bind", "0.0.0.0"),
            .port = toml::find_or<std::uint16_t>(server, "port", 8000),
            .database_path = toml::find_or<std::string>(
              server, "database_path", "main.db"),
            .jwt_secret = toml::find_or<std::string>(jwt, "secret", "secret"),
            .jwt_issuer = toml::find_or<std::string>(jwt, "issuer", "issuer"),
            .jwt_expire_after = toml::find_or<std::uint64_t>(
              jwt,
              "expire_after",
              duration_cast<milliseconds>(days{ 30 }).count()), // NOLINT
            .swagger_res_path = toml::find_or<std::string>(
              swagger,
              "res_path",
              "/usr/local/share/" PROJECT_NAME "/oatpp-swagger-res"),
        };
    }
};

template<>
struct [[maybe_unused]] into<Config>
{
    [[maybe_unused]] static toml::value
    into_toml(Config const& config)
    {
        // When writing to file values will be written last to first
        // thus the reverse order.
        return toml::value(
          { { "server",
              {
                { "swagger",
                  {
                    { "res_path", config.swagger_res_path },
                  } },
                { "jwt",
                  {
                    { "expire_after", config.jwt_expire_after },
                    { "issuer", config.jwt_issuer },
                    { "secret", config.jwt_secret },
                  } },
                { "database_path", config.database_path },
                { "port", config.port },
                { "bind", config.bind },
              } } });
    }
};

} // namespace toml
