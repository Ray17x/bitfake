#include "Outputs.hpp"
using namespace Outputs;
#include <vector>
#include <filesystem>
#include "Configuration.hpp"
#include <fstream>
#include <thread>
#include <chrono>
#include "MainFunctions.hpp"
using namespace MainFunctions;

GLOBAL_CONFIG GlobalConf;

int main(int argc, char* argv[]) {
    std::vector<std::string_view> args(argv, argv + argc);
    args.size() == 0 ? err("No arguments.") : void();

    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "-i" || args[i] == "--input") {
            i + 1 >= args.size() ? err("Nothing after --input flag? Check your command.") : void();
            !std::filesystem::exists(args[i + 1]) ? err("File path does not exist.") : void();
            GlobalConf.INPUT_PATH = args[i + 1];
            log("File Path " + std::string(args[i]) + " added to INPUT_PATH");
            ++i;

        }
        if (args[i] == "-v" || args[i] == "--verbose") {
            GlobalConf.verbose = true;
        }

        if (args[i] == "-o" || args[i] == "--output") {
            if (i + 1 >= args.size()) err("Nothing after --output flag? Check your command.");

            std::filesystem::path tmp_path{ std::string(args[i + 1]) };
            GlobalConf.OUTPUT_PATH = tmp_path; // preserve exact filename user provided

            std::filesystem::path parent = tmp_path.parent_path();
            if (parent.empty()) parent = std::filesystem::current_path();

            std::filesystem::path found = parent;
            while (!found.empty() && !std::filesystem::exists(found)) found = found.parent_path();
            if (found.empty()) err("No existing parent dir for this file.");
            parent = found;
            
            if (!std::filesystem::is_directory(parent)) err("Parent exists but is not a directory: " + parent.string());
            GlobalConf.OUTPUT_DIR = parent;
            ++i;

            // OUTPUT path keeps file name
            // OUTPUT dir is where it will write
        }
    }

    for (size_t j = 0; j < args.size(); ++j) {
        // Main functions go here

        if (args[j] == "-h" || args[j] == "--help") {
            log("help command.");
            std::cout
            << "Usage: bitf -i <input dir/file> [options] -o <output file>\n (Highly recommended to format it like this to easily share your command! [and most likely way to work lol])\n"
            << "-h, --help\t help command.\n"
            << "-i, --input\t inputs <file> (you must use this :( i dont know how else to do it!)\n"
            << "-o, --output\t outputs to <file> (not required, but really recommeneded for grabbing codec infos for albums or playlists and not singles.)\n"
            << "-v, --verbose\t enables verbose\n"
            << "--version\t returns version number (ty for using this tool btw)"
            << std::endl;
        }

        if (args[j] == "--version") {
            std::cout << "Version: " << GlobalConf.version << std::endl;
            std::cout << "Thank you for using this tool!" << std::endl;
        }

        // main features ig

        if (args[j] == "--extract-codec-info" || args[j] == "-EAI") {
            if (GlobalConf.INPUT_PATH.empty()) err("NO input path specified.");
            if (std::filesystem::is_directory(GlobalConf.INPUT_PATH) && GlobalConf.OUTPUT_PATH.empty()) {
                warn("You are extracting codec info for a directory but have not specified an output file. It is highly recommended to specify an output file when extracting codec info for multiple files.");
                warn("Ctrl + C to abort please!! :*(");
                // Count down 5 seconds
                for (int k = 5; k > 0; --k) {
                    std::cout << k << "..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                log("Proceeding :*(");
            }

            // log("I LIED!!! NOT YET THIS IS A TEST BUILD!!");
            // ^^^ he died because we implemented this feature

            MainFunctions::BasicCodecInfo info = MainFunctions::ExtractBasicCodec(GlobalConf.INPUT_PATH);
            log("Codec Info extracted!");
        }

        if (args[j] == "--extract-advanced-metadata" || args[j] == "-EAM") {
            if (GlobalConf.INPUT_PATH.empty()) err("NO input path specified.");
            if (std::filesystem::is_directory(GlobalConf.INPUT_PATH) && GlobalConf.OUTPUT_PATH.empty()) {
                warn("You are extracting advanced metadata for a directory but have not specified an output file. It is highly recommended to specify an output file when extracting advanced metadata for multiple files.");
                warn("Ctrl + C to abort please!! :*(");
                // Count down 5 seconds
                for (int k = 5; k > 0; --k) {
                    std::cout << k << "..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                log("Proceeding :*(");
            }

            MainFunctions::AdvancedMetaData data = MainFunctions::ExtractAdvancedMeta(GlobalConf.INPUT_PATH);
        }

        if (args[j] == "--extract-replaygain-info" || args[j] == "-ERGI") {
            if (GlobalConf.INPUT_PATH.empty()) err("NO input path specified.");
            if (std::filesystem::is_directory(GlobalConf.INPUT_PATH) && GlobalConf.OUTPUT_PATH.empty()) {
                warn("You are extracting replaygain info for a directory but have not specified an output file. It is highly recommended to specify an output file when extracting replaygain info for multiple files.");
                warn("Ctrl + C to abort please!! :*(");
                // Count down 5 seconds
                for (int k = 5; k > 0; --k) {
                    std::cout << k << "..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                log("Proceeding :*(");
            }

            MainFunctions::ReplayGainInfo info = MainFunctions::ExtractReplayGain(GlobalConf.INPUT_PATH);
        }

    }

    return EXIT_SUCCESS;
}