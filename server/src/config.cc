#include "config.hh"

#include <cstdlib>
#include <filesystem>
#include <fstream>

#include <oatpp/core/base/Environment.hpp>

namespace server
{

namespace detail
{

/**
   @brief Find directory for storing config files.
 */
std::filesystem::path
get_config_home()
{
    char const* xdg_config_home_dir = std::getenv("XDG_CONFIG_HOME");
    if(xdg_config_home_dir != nullptr) {
        return xdg_config_home_dir;
    }

    char const* appdata_dir = std::getenv("APPDATA");
    if(appdata_dir != nullptr) {
        return appdata_dir;
    }

    char const* home_dir = std::getenv("HOME");
    if(home_dir != nullptr) {
        return std::filesystem::path{home_dir} / ".config";
    }

    throw std::runtime_error{"XDG_CONFIG_HOME or APPDATA environment variable not present."};
}

static constexpr
char const* CONFIG_FILE_DIR = "parking_space_sharing";

static constexpr
char const* CONFIG_FILE_NAME = "config.toml";

/**
   @brief Returns platform specific config path, and ensures that it exists.
 */
std::filesystem::path
get_and_ensure_config_path()
{
    auto config_path = get_config_home() / CONFIG_FILE_DIR;
    // Make sure that the directory exists
    std::filesystem::create_directory(config_path);

    config_path /= CONFIG_FILE_NAME;
    return config_path;
}

// This also dumps the default config file to the default location
// if no config file was found.
server::config
get_config()
{
    server::config config{};

    auto config_path = get_and_ensure_config_path();
    if(std::filesystem::exists(config_path)) {
        OATPP_LOGI("Server", "\tUsing \"%s\" as configuration file.", config_path.c_str());
        config = toml::get<server::config>(toml::parse(config_path));
    } else {
        OATPP_LOGI("Server", "\tCould not find the config file, "
                             "dumping the default values.\n"
                             "The new config file will be placed in \"%s\"", config_path.c_str());
        // This will construct the config with default values.
        // See config.hh from_toml()
        config = toml::get<server::config>(toml::value{});
    }
    toml::value config_toml = config;

    // Write the config to file even when one was already there.
    std::ofstream{
        config_path,
        std::ios_base::in |
        std::ios_base::trunc
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
