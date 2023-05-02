#include "Config.hh"
namespace server {

/**
   @brief Find directory for storing config files.
 */

std::filesystem::path
getConfigHome()
{
    char const* xdg_config_home_dir = std::getenv("XDG_CONFIG_HOME");
    if (xdg_config_home_dir != nullptr) {
        return xdg_config_home_dir;
    }

    char const* appdata_dir = std::getenv("APPDATA");
    if (appdata_dir != nullptr) {
        return appdata_dir;
    }

    char const* home_dir = std::getenv("HOME");
    if (home_dir != nullptr) {
        return std::filesystem::path{ home_dir } / ".config";
    }

    throw std::runtime_error{
        "XDG_CONFIG_HOME or APPDATA environment variable not present."
    };
}

static constexpr char const* CONFIG_FILE_DIR = "parking_space_sharing";
static constexpr char const* CONFIG_FILE_NAME = "config.toml";

/**
   @brief Returns platform specific config path, and ensures that it exists.
 */

std::filesystem::path
getAndEnsureConfigPath()
{
    auto config_path = getConfigHome() / CONFIG_FILE_DIR;
    // Make sure that the directory exists
    std::filesystem::create_directory(config_path);

    config_path /= CONFIG_FILE_NAME;
    return config_path;
}

// This also dumps the default config file to the default location
// if no config file was found.
Config
getConfig()
{
    Config config{};

    auto config_path = getAndEnsureConfigPath();

    if (std::filesystem::exists(config_path)) {
        OATPP_LOGI("Server",
                   "\tUsing \"%s\" as configuration file.",
                   config_path.c_str())

        config = toml::get<Config>(toml::parse(config_path));
    } else {
        OATPP_LOGI("Server",
                   "\tCould not find the config file, "
                   "dumping the default values.\n"
                   "The new config file will be placed in \"%s\".\n",
                   config_path.c_str())
        // This will construct the config with default values.
        // See Config.hh from_toml()
        config = toml::get<Config>(toml::value{});
    }

    toml::value const config_toml = config;
    std::cout << "Current config:\n" << config_toml;

    // Write the config to file even when one was already there.
    std::ofstream{ config_path, std::ios_base::in | std::ios_base::trunc }
      << config_toml;

    return config;
}

Config const&
Config::getInstance()
{
    static Config const config{ getConfig() };

    return config;
}

} // namespace server