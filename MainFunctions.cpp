#include "MainFunctions.hpp"
using namespace MainFunctions;
#include "Outputs.hpp"
using namespace Outputs;
#include "Configuration.hpp"
#include <filesystem>
#include <optional>
#include <cstdio>
#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

extern "C" {
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

extern GLOBAL_CONFIG GlobalConf;




// extern BasicCodecInfo ExtractBasicCodec(const std::filesystem::path& filepath);
// extern AdvancedMetaData ExtractAdvancedMeta(const std::filesystem::path& filepath);
// extern ReplayGainInfo ExtractReplayGain(const std::filesystem::path& filepath);


// extern void Convert(const std::filesystem::path& input_path, const std::string& target, const std::optional<std::filesystem::path>& output_path = std::nullopt);
// extern float CalculateReplayGainPerTrack(const std::filesystem::path& input_path);
// extern float CalculateReplayGainPerAlbum(const std::filesystem::path& input_path);

BasicCodecInfo MainFunctions::ExtractBasicCodec(const std::filesystem::path& filepath) {
    if (ValidateAudioFile(filepath) && !std::filesystem::is_directory(filepath)) {
            BasicCodecInfo info;
        
        // ok prob best to use popen

        std::string command = "ffprobe -v error -select_streams a:0 -show_entries stream=codec_name,bit_rate,sample_rate,channels:format_tags=title -of default=noprint_wrappers=1:nokey=1 \"" + filepath.string() + "\"";

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            err("ffprobe did not work, please ensure you have ffmpeg installed on your system.");
            return info;
        }

        std::array<char, 128> buffer;
        std::string result;
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }

        // flac
        // 44100
        // 2
        // N/A
        // Linda Claire
        // Imaginary data for the BasicCodecInfo struct, popen returns the info in this order.
        // Seperate thee data back into the BasicCodecInfo struct.

        pclose(pipe);
        std::istringstream iss(result);
        std::string line;
        int line_number = 0;
        while (std::getline(iss, line)) {
            switch (line_number) {
                case 0:
                    info.codec_name = line;
                    break;
                case 1:
                    if (line != "N/A" && !line.empty()) {
                        try {
                            info.bitrate_kbps = std::stoi(line) / 1000; // convert to kbps
                        } catch (...) {
                            info.bitrate_kbps = 0;
                        }
                    } else {
                        info.bitrate_kbps = 0;
                    }
                    break;
                case 2:
                    if (line != "N/A" && !line.empty()) {
                        try {
                            info.sample_rate_hz = std::stoi(line);
                        } catch (...) {
                            info.sample_rate_hz = 0;
                        }
                    } else {
                        info.sample_rate_hz = 0;
                    }
                    break;
                case 3:
                    if (line != "N/A" && !line.empty()) {
                        try {
                            info.channels = std::stoi(line);
                        } catch (...) {
                            info.channels = 0;
                        }
                    } else {
                        info.channels = 0;
                    }
                    break;
                case 4:
                    info.song_title = line;
                    break;
                default:
                    break;
            }
            line_number++;
        }
        info.file_path = filepath.string();
        if (std::string(GlobalConf.OUTPUT_DIR).empty() == true) {
                    warn("You did not specify an output directory/file so all output will be jammed here.");
                    std::cout
                    << std::left << std::setw(20) << "Title:" << info.song_title << "\n"
                    << std::left << std::setw(20) << "File Path:" << info.file_path << "\n"
                    << std::left << std::setw(20) << "Codec Name:" << info.codec_name << "\n"
                    << std::left << std::setw(20) << "Bitrate (kbps):" << info.bitrate_kbps << "\n"
                    << std::left << std::setw(20) << "Sample Rate (Hz):" << info.sample_rate_hz << "\n"
                    << std::left << std::setw(20) << "Channels:" << info.channels << "\n"
                    << "----------------------------------------" << std::endl;
        } else if (std::filesystem::exists(GlobalConf.OUTPUT_PATH) || std::filesystem::is_directory(GlobalConf.OUTPUT_DIR)) {
            // write/append to file
            std::ofstream outputfile;
            outputfile.open(GlobalConf.OUTPUT_PATH, std::ios::app);
            if (!outputfile.is_open()) {
                err("Failed to open output file for writing codec info.");
            }
            outputfile
            << std::left << std::setw(20) << "Title:" << info.song_title << "\n"
            << std::left << std::setw(20) << "File_Path:" << info.file_path << "\n"
            << std::left << std::setw(20) << "Codec_Name:" << info.codec_name << "\n"
            << std::left << std::setw(20) << "Bitrate_(kbps):" << info.bitrate_kbps << "\n"
            << std::left << std::setw(20) << "Sample_Rate_(Hz):" << info.sample_rate_hz << "\n"
            << std::left << std::setw(20) << "Channels:" << info.channels << "\n"
            << "----------------------------------------" << "\n";
            outputfile.close();
        }
        return info;
    } else if (std::filesystem::exists(filepath) && std::filesystem::is_directory(filepath)) {

        // Collect all valid audio files first
        std::vector<std::filesystem::path> audio_files;
        for (const auto& entry : std::filesystem::directory_iterator(filepath)) {
            if (ValidateAudioFile(entry.path())) {
                audio_files.push_back(entry.path());
            } else {
                warn("Skipping invalid audio file in directory: " + entry.path().string());
            }
        }

        // Sort the files by filename
        std::sort(audio_files.begin(), audio_files.end());

        // Process them in sorted order
        for (const auto& file : audio_files) {
            auto info = ExtractBasicCodec(file);
        }

        BasicCodecInfo dummy;
        return dummy; // Return empty info when processing directory
    } else {
        err("Invalid audio file provided for ExtractBasicCodec.");
        BasicCodecInfo dummy;
        return dummy;
    }
}