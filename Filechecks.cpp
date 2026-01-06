#include <iostream>
#include "Output.hpp"
using namespace Output;
#include <vector>
#include <string_view>
#include "Conf.hpp"
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <charconv>
#include "Filechecks.hpp"
using namespace FileChecks;

/*
    // ERROR() WILL RETURN EXIT_FAILURE WHEN CALLED!!
*/

// Define the global Conf instance
Configuration Conf;

namespace FileChecks {
    // Safe integer parser: returns 0 on parse failure
    static int safe_stoi(const std::string& s) {
        if (s.empty()) return 0;
        // strip common separators and whitespace
        std::string tmp;
        tmp.reserve(s.size());
        for (char c : s) {
            if (c == ',' || c == ' ' || c == '\t' || c == '\r' || c == '\n') continue;
            tmp.push_back(c);
        }
        int value = 0;
        auto res = std::from_chars(tmp.data(), tmp.data() + tmp.size(), value);
        if (res.ec == std::errc()) return value;
        return 0;
    }
    AudioCodecInfo ExtractAudioCodecInfo(const std::filesystem::path& file) {
        AudioCodecInfo Information;
        std::string cmd = "ffprobe -v error -select_streams a:0 -show_entries stream=codec_name,bit_rate,sample_rate,channels -of default=noprint_wrappers=1:nokey=1 \"" + file.string() + "\"";

        // Check if it's a directory first (before audio file validation)
        if (std::filesystem::is_directory(file)) {
            Conf.debug_mode ? debug_log("ExtractAudioCodecInfo: Path is a dir, recursion WILL happen...") : void();
            warn("Path is a directory, not a file: " + file.string());
            for (auto& entry : std::filesystem::directory_iterator(file)) {
                if (!ValidateAudioFileExistance(entry.path())) {
                    Conf.debug_mode ? debug_log("EACI is Skipping non-audio file: " + entry.path().string()) : void();
                    Conf.verbose_mode ? warn("Skipping non-audio file: " + entry.path().string()) : void();
                    continue;
                } else {
                    Conf.verbose_mode ? info("Processing Audio File(s): " + entry.path().string()) : void();
                    // Recursive call
                    ExtractAudioCodecInfo(entry.path());
                }
            }
            return Information;
        }

        // Validate audio file
        if (!ValidateAudioFileExistance(file)) {
            Conf.debug_mode ? debug_log("ExtractAudioCodecInfo: File does not exist/is not a (valid)/supported audio file. Sorry if its the 2nd reason :*(.") : void();
            error("File does not exist: " + file.string());
            return Information;
        }

        // Process audio file
        FILE* pipe = popen(cmd.c_str(), "r");
        Conf.debug_mode ? debug_log("Pipe opened! Command should've ran here.") : void();
        Conf.debug_mode ? debug_log("Command Output: " + std::to_string(reinterpret_cast<uintptr_t>(pipe))) : void();
        if (!pipe) {
            Conf.debug_mode ? debug_log("ExtractAudioCodecInfo: Check if you have ffmpeg/ffprobe installed") : void();
            error("Failed to run EACI. Use -d for more info");
            return Information;
        }

        char buffer[128];
        std::vector<std::string> results;
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            std::string line(buffer);
            // Remove trailing newline
            if (!line.empty() && line.back() == '\n') {
                line.pop_back();
            }
            results.push_back(line);
        }
        pclose(pipe);

        Information.codec_name = results.size() > 0 ? results[0] : "";
        Information.bitrate = results.size() > 1 ? safe_stoi(results[1]) : 0;
        Information.sample_rate = results.size() > 2 ? safe_stoi(results[2]) : 0;
        Information.channels = results.size() > 3 ? safe_stoi(results[3]) : 0;
        Information.valid = !Information.codec_name.empty();

        return Information;
    } // ExtractAudioCodecInfo

    AudioMetadata ExtractAudioMetadata(const std::filesystem::path& file) {
        AudioMetadata metadata;
        // Practically same logic as ExtractAudioCodecInfo but different ffprobe command
        std::string cmd = "ffprobe -v error -show_entries format_tags=album,album_artist,artist,date,disc,disc_total,replaygain_album_gain,replaygain_album_peak,replaygain_track_gain,replaygain_track_peak,title,track,track_total,genre,comment -of default=noprint_wrappers=1:nokey=1 \"" + file.string() + "\"";
        
        return metadata;
    }
}