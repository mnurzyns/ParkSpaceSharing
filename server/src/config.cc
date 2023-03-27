#include "config.hh"

#include <fstream>
#include <iostream>

namespace server
{

namespace detail
{

// This also dumps the default config file to the default location
// if no config file was found.
//
// TODO(Piotr Stefański): Search for config files in directories like
//                        $XDG_CONFIG_HOME/parkign_space_sharing/config.toml
//                        instead of just ./config.toml
::server::config
get_config()
{
    ::server::config config{};

    try {
        std::cout << "Found config file 'config.toml'\n";
        config = ::toml::get<::server::config>(::toml::parse("config.toml"));
    } catch(std::runtime_error&) {
        // Catch file not found exception.
        // Unfortunately there is no more specific class to catch.

        std::cout << "Could not find the config file,\n"
                     "dumping the default values.\n";
        // This will construct the config with default values.
        // See config.hh from_toml()
        config = ::toml::get<::server::config>(::toml::value{});
    }

    ::toml::value config_toml = config;
    std::cout << "Current config:\n" << config_toml;

    // Write the config to file even when one was already there.
    std::ofstream{
        "./config.toml",
        ::std::ios_base::in |
        ::std::ios_base::trunc
    } << config_toml;


    return config;
};

} // namespace detail

config const&
config::get_instance()
{
    static
    config const
    config{ detail::get_config() };

    return config;
}

} // namespace server
