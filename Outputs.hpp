#ifndef OUTPUTS_HPP
#define OUTPUTS_HPP

#include <iostream>

namespace Outputs {
    class Colors {
        public:
            static constexpr const char* RESET       = "\033[0m";
            static constexpr const char* RED         = "\033[31m";
            static constexpr const char* GREEN       = "\033[32m";
            static constexpr const char* YELLOW      = "\033[33m";
            static constexpr const char* BLUE        = "\033[34m";
            static constexpr const char* MAGENTA     = "\033[35m";
            static constexpr const char* CYAN        = "\033[36m";
            static constexpr const char* WHITE       = "\033[37m";
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

    inline void warn(const std::string_view& msg) {
        std::cout
        << Colors::BOLD_YELLOW
        << "[?] "
        << Colors::RESET
        << Colors::YELLOW
        << msg
        << Colors::RESET
        << std::endl;
    }

    inline void log(const std::string_view& msg) {
        std::cout
        << Colors::BOLD_WHITE
        << "[LOG] "
        << Colors::RESET
        << Colors::WHITE
        << msg
        << Colors::RESET
        << std::endl;
    }

    inline void err(const std::string_view& msg) {
        std::cout
        << Colors::BOLD_RED
        << "[err?] "
        << Colors::RESET
        << Colors::RED
        << msg
        << Colors::RESET
        << std::endl;
        
        std::cout
        << Colors::BOLD_RED
        << "The program will now exit."
        << Colors::RESET
        << std::endl;
        exit(EXIT_FAILURE);
    }


    // tbf only to be used while developing
    inline void scram() {
        std::cout
        << Colors::BOLD_MAGENTA
        << "The program will now exit (Scram function)"
        << Colors::RESET
        << std::endl;
        exit(EXIT_FAILURE);
    }
}

#endif // OUTPUTS_HPP