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
    ::std::string bind;
    ::std::uint16_t port;

    ::std::string info_url;
    ::std::string info_description;

    static
    ::server::config const&
    get_instance();
};

} // namespace server

namespace toml
{

template<>
struct from<::server::config>
{
    static
    ::server::config from_toml(::toml::value const& val)
    {
        auto server = ::toml::find_or(val, "server", {});
        auto server_info = ::toml::find_or(server, "info", {});

        return ::server::config
        {
            .bind = ::toml::find_or<::std::string>(server, "bind", "0.0.0.0"),
            .port = ::toml::find_or<::std::uint16_t>(server, "port", 8000),

            .info_url = ::toml::find_or<::std::string>(server_info, "url", "http://localhost:8000"),
            .info_description = ::toml::find_or<::std::string>(server_info, "description", "server on localhost"),
        };
    }
};

template<>
struct into<::server::config>
{
    static
    ::toml::value into_toml(::server::config const& config)
    {
        // When writing to file values will be written last to first
        // thus the reverse order.
        return ::toml::value(
                {{"server",{
                    {"info", {
                        {"description", config.info_description},
                        {"url",         config.info_url},
                    }},
                    {"port", config.port},
                    {"bind", config.bind}
                }}}
        );
    }
};

} // namespace toml
