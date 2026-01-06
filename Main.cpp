#include <iostream>
#include "Output.hpp"
using namespace Output;
#include <vector>
#include <string_view>
#include "Conf.hpp"
// Configuration Conf;
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include "Filechecks.hpp"
using namespace FileChecks;

/*
    // ERROR() WILL RETURN EXIT_FAILURE WHEN CALLED!!
*/

int main(int argc, char* argv[]) {
    std::vector<std::string_view> args(argv + 1, argv + argc);

    // First Pass looks for in, out, verbose, & debug
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "-f" || args[i] == "--file") {
            i + 1 >= args.size() ? error("Missing File. Use -h or --help for help.") : void();
            !std::filesystem::exists(args[i + 1]) ? error("File path does not exist.") : void();
            Conf.InputPath = args[i + 1];
            ++i;
            // debug_log("YAY it worked!");
        }
        if (args[i] == "-v" || args[i] == "--verbose") {
            Conf.verbose_mode = true;
        }
        if (args[i] == "-d" || args[i] == "--debug") {
            Conf.debug_mode = true;
        }
        if (args[i] == "-o" || args[i] == "--output") {
            if (i + 1 >= args.size()) {
                warn("Nothing after -o. File may be created automatically");
                continue;
            }

            std::filesystem::path target(args[i + 1]);
            std::filesystem::path parent = target.has_parent_path() ? target.parent_path() : std::filesystem::current_path();

            if (!std::filesystem::exists(parent)) {
                error("Output directory does not exist: " + parent.string() + " Use -h or --help for help.");
            }

            if (std::filesystem::is_directory(target)) {
                target /= "output.txt"; // default file name when only a directory is provided
                // To be changed to the folder name with .txt extension later..
            }

            if (!std::filesystem::exists(target)) {
                std::ofstream create_file(target);
                if (!create_file) {
                    error("Failed to create output file: " + target.string());
                }
            }

            Conf.OutputPath = target;
            ++i;
        }
    }

    for (size_t arg = 0; arg < args.size(); ++arg) {
        if (args[arg] == "-h" || args[arg] == "--help") {
            std::cout << "Bitfake Help Menu\n"
                      << "Usage: bitfake [options]\n\n"
                      << "Options:\n"
                      << "  -f, --file <path>       Specify input file path (required)\n"
                      << "  -o, --output <path>     Specify output file path (optional)\n"
                      << "  -v, --verbose           Enable verbose mode (optional)\n"
                      << "  -d, --debug             Enable debug mode (optional)\n"
                      << "  -h, --help              Show this help message\n"
                      << "  --version               Show version information\n";
            warn("It is HIGHLY recommended to provide an output file using -o for more than like 5 files in a folder.");
            return EXIT_SUCCESS;
        }

        if (args[arg] == "--version") {
            std::cout << "Bitfake Version: " << Conf.ver << std::endl;
            success("THANK YOU FOR USING BITFAKE <33");
            return EXIT_SUCCESS;
        }

        // if (args[arg] == "-t") {
        //     // Print out all current configurations for testing
        //     info("Current Configuration:");
        //     info("Verbose Mode: " + std::string(Conf.verbose_mode ? "Enabled" : "Disabled"));
        //     info("Debug Mode: " + std::string(Conf.debug_mode ? "Enabled" : "Disabled"));
        //     info("Input Path: " + Conf.InputPath.string());
        //     info("Output Path: " + Conf.OutputPath.string());
        // }

        // Additional Operations here...

        if (args[arg] == "--ExtractAudioCodecInfo" || args[arg] == "--EACI") {
            try {
                AudioCodecInfo Information = ExtractAudioCodecInfo(Conf.InputPath);
                info("Codec Name: " + Information.codec_name);
                info("Bitrate: " + std::to_string(Information.bitrate) + " kbps");
                info("Sample Rate: " + std::to_string(Information.sample_rate) + " Hz");
                info("Channels: " + std::to_string(Information.channels));
                success("Audio Codec Info extracted successfully.");
                exit(EXIT_SUCCESS);
            } catch (const std::exception& e) {
                error(std::string("Failed to extract audio codec info: ") + e.what());
            }
        }   


    }
}