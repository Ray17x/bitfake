#include "Outputs.hpp"
using namespace Outputs;
#include <vector>
#include <filesystem>
#include "Configuration.hpp"
#include <fstream>

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
            // OUTPUT dir is where it will write.
        }
    }

    for (size_t j = 0; j < args.size(); ++j) {
        // Main functions go here.

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
        
    }

    return EXIT_SUCCESS;
}