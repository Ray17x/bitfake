#include <filesystem>


class Configuration {
    public:
    bool verbose_mode = false;
    bool debug_mode = false;
    std::filesystem::path InputPath;
    std::filesystem::path OutputPath;
    std::string_view ver = "0.0.1-a";
};

/// @brief Global configuration object instance
/// @details Manages application configuration settings and parameters
/// @note This is a global singleton instance of the Configuration class

extern Configuration Conf;

// Definition (should be in exactly one .cpp file)
// Configuration Conf;