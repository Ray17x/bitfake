#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <filesystem>

class GLOBAL_CONFIG {
    public:
    bool verbose = false;
    std::filesystem::path INPUT_PATH;
    std::filesystem::path OUTPUT_PATH;
    std::filesystem::path OUTPUT_DIR;
    std::string_view version = "0.0.1-a"; // Don't edit me pls :D
};


#endif // CONFIGURATION_HPP