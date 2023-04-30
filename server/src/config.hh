#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <toml.hpp>

namespace server
{

// When changing value types of members
// also update template argument of from_toml().
struct config
{
    std::string bind;
    std::uint16_t port;

    std::string database_path;

    std::string jwt_secret;
    std::string jwt_issuer;
    std::uint64_t jwt_expire_after;

    std::string swagger_res_path;

    static
    server::config const&
    get_instance();
};

} // namespace server

namespace toml
{

template<>
struct from<server::config>
{
    static
    server::config from_toml(toml::value const& val)
    {
        auto server = toml::find_or(val, "server", {});
        auto jwt = toml::find_or(server, "jwt", {});
        auto swagger = toml::find_or(server, "swagger", {});

        return server::config
        {
            .bind = toml::find_or<std::string>(server, "bind", "0.0.0.0"),
            .port = toml::find_or<std::uint16_t>(server, "port", 8000),
            .database_path = toml::find_or<std::string>(server, "database_path", "main.sqlite3"),
            .jwt_secret = toml::find_or<std::string>(jwt, "secret", "secret"),
            .jwt_issuer = toml::find_or<std::string>(jwt, "issuer", "issuer"),
            .jwt_expire_after = toml::find_or<std::uint64_t>(jwt, "expire_after", 30*24*60*60), // 30 days
            .swagger_res_path = toml::find_or<std::string>(jwt, "res_path", "/usr/local/share/" PSS_PROJECT_NAME "/oatpp-swagger-res"),
        };
    }
};

template<>
struct into<server::config>
{
    static
    toml::value into_toml(server::config const& config)
    {
        // When writing to file values will be written last to first
        // thus the reverse order.
        return toml::value(
                {{"server",{
                    {"swagger", {
                        {"res_path", config.swagger_res_path},
                    }},
                    {"jwt", {
                        {"expire_after", config.jwt_expire_after},
                        {"issuer", config.jwt_issuer},
                        {"secret", config.jwt_secret},
                    }},
                    {"database_path", config.database_path},
                    {"port", config.port},
                    {"bind", config.bind},
                }}}
        );
    }
};

} // namespace toml
