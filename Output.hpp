#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <iostream>
#include <cstdlib>

/*
    // This File contains functions for descriptive Outputs
    // and uses references from public open source projects
    // for the colors. 
*/

namespace Output {
    class Colors {
    public:

        // Regular Colors

        static constexpr const char* RESET       = "\033[0m";
        static constexpr const char* RED         = "\033[31m";
        static constexpr const char* GREEN       = "\033[32m";
        static constexpr const char* YELLOW      = "\033[33m";
        static constexpr const char* BLUE        = "\033[34m";
        static constexpr const char* MAGENTA     = "\033[35m";
        static constexpr const char* CYAN        = "\033[36m";
        static constexpr const char* BOLD        = "\033[1m";
        static constexpr const char* UNDERLINE   = "\033[4m";

        // Bold

        static constexpr const char* BOLD_RED     = "\033[1;31m";
        static constexpr const char* BOLD_GREEN   = "\033[1;32m";
        static constexpr const char* BOLD_YELLOW  = "\033[1;33m";
        static constexpr const char* BOLD_BLUE    = "\033[1;34m";
        static constexpr const char* BOLD_MAGENTA = "\033[1;35m";
        static constexpr const char* BOLD_CYAN    = "\033[1;36m";
        static constexpr const char* BOLD_WHITE   = "\033[1;37m";
    };

    inline void error(const std::string_view& msg = "") {
        if (msg == "") {
            // Default Error Message
            // Usage: error() and prints message in red
            std::cout 
            << Colors::BOLD_RED 
            << "[!!!] " 
            << Colors::RESET 
            << Colors::RED 
            << "Something went wrong!" 
            << Colors::RESET 
            << std::endl;
            exit(EXIT_FAILURE);
        } else {
            // Non-default Error Message
            // Usage: error("<msg>") and prints message in red
            std::cout
            << Colors::BOLD_RED 
            << "[!!!] " 
            << Colors::RESET 
            << Colors::RED
            << msg
            << Colors::RESET
            << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    inline void success(const std::string_view& msg = "") {
    if (msg == "") {
        // Default Success Message
        // Usage: success() and prints message in green
        std::cout 
        << Colors::BOLD_GREEN 
        << "[+++] " 
        << Colors::RESET 
        << Colors::GREEN 
        << "Operation completed successfully!" 
        << Colors::RESET 
        << std::endl;
    } else {
        // Non-default Success Message
        // Usage: success("<msg>") and prints message in green
        std::cout
        << Colors::BOLD_GREEN 
        << "[+++] " 
        << Colors::RESET 
        << Colors::GREEN
        << msg
        << Colors::RESET
        << std::endl;
    }
    }

    inline void info(const std::string_view& msg) {
        // Info Message
        // Usage: info("<msg>") and prints message in blue
        std::cout
        << Colors::BOLD_BLUE 
        << "[i] " 
        << Colors::RESET 
        << Colors::BLUE
        << msg
        << Colors::RESET
        << std::endl;
        // There should be no default message for info.
    }

    inline void warn(const std::string_view& msg) {
        // Warning Message
        // Usage: warn("<msg>") and prints message in yellow
        std::cout
        << Colors::BOLD_YELLOW 
        << "[?] " 
        << Colors::RESET 
        << Colors::YELLOW
        << msg
        << Colors::RESET
        << std::endl;
        // There should be no default message for warn.
    }

    // To the fucking idiots that suggested std::print
    // MY SHIT IS ALL ERRORS FUCK YOU!!!!

    inline void debug_log(const std::string_view& msg) {
        std::cout
        << Colors::BOLD_MAGENTA
        << "[dbl] "
        << Colors::RESET
        << Colors::MAGENTA
        << msg
        << Colors::RESET
        << std::endl;
    }
}

#endif // OUTPUT_HPP