#include "MainFunctions.hpp"
using namespace MainFunctions;
#include "Outputs.hpp"
using namespace Outputs;
#include "Configuration.hpp"
#include <filesystem>
#include <cstdio>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

extern GLOBAL_CONFIG GlobalConf;

BasicCodecInfo MainFunctions::ExtractBasicCodec(const std::filesystem::path& filepath) {
    if (ValidateAudioFile(filepath) && !std::filesystem::is_directory(filepath)) {
        BasicCodecInfo info;

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

        // fleshwater my beloved

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
        warn("You did not specify an output directory/file so all output will be jammed here.");
        // Moved it outside
        if (std::string(GlobalConf.OUTPUT_DIR).empty() == true) {
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
} // extern BasicCodecInfo ExtractBasicCodec(const std::filesystem::path& filepath);

AdvancedMetaData MainFunctions::ExtractAdvancedMeta(const std::filesystem::path& filepath) {
    if (ValidateAudioFile(filepath) && !std::filesystem::is_directory(filepath)) {
            AdvancedMetaData data;

    // struct AdvancedMetaData {

    //     std::string TITLE;
    //     std::string ARTIST;
    //     std::string ALBUM;
    //     std::string ALBUM_ARTIST;
    //     std::string GENRE;
    //     int TRACK_NUMBER;
    //     int TOTAL_TRACKS;
    //     int DISC_NUMBER;
    //     int TOTAL_DISCS;
    //     int YEAR;
    //     std::string DATE;
    //     float REPLAYGAIN_TRACK_GAIN;
    //     float REPLAYGAIN_ALBUM_GAIN;
    //     float REPLAYGAIN_TRACK_PEAK;
    //     float REPLAYGAIN_ALBUM_PEAK;
    //     std::string COMMENT;
    //     std::string codec_name;
    //     int bitrate_kbps;
    //     int sample_rate_hz;
    //     int channels;
    // };

    // Command needs to extract everything above
    // I think this works
    std::string command = "ffprobe -v error -select_streams a:0 -show_entries format_tags=title,artist,album,album_artist,genre,track,disc,date,year,comment -show_entries stream=codec_name,bit_rate,sample_rate,channels -of default=noprint_wrappers=1 \"" + filepath.string() + "\"";

    // codec_name=flac
    // sample_rate=48000
    // channels=2
    // bit_rate=N/A
    // TAG:ALBUM=2000: In Search of the Endless Sky
    // TAG:ALBUM_ARTIST=Fleshwater
    // TAG:ARTIST=Fleshwater
    // TAG:COMMENT=comment
    // TAG:DATE=2025-09-05
    // TAG:disc=1
    // TAG:GENRE=Alternative Metal
    // TAG:TITLE=Drowning Song
    // TAG:track=1
    // TAG:YEAR=2025

    // popen again
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) err("ffprobe did not work, please ensure you have ffmpeg installed on your system.");
    std::array<char, 256> buffer;
    std::string result;
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);

    std::istringstream iss(result);
    std::string line;
    int line_number = 0;

    while (std::getline(iss, line)) {
        if (line.find('=') != std::string::npos) {
            std::string key = line.substr(0, line.find('='));
            std::string value = line.substr(line.find('=') + 1);

            if (key == "codec_name") {
                data.codec_name = value;
            } else if (key == "bit_rate") {
                try {
                    data.bitrate_kbps = (value != "N/A" && !value.empty()) ? std::stoi(value) / 1000 : 0;
                } catch (...) {
                    data.bitrate_kbps = 0;
                }
            } else if (key == "sample_rate") {
                try {
                    data.sample_rate_hz = (value != "N/A" && !value.empty()) ? std::stoi(value) : 0;
                } catch (...) {
                    data.sample_rate_hz = 0;
                }
            } else if (key == "channels") {
                try {
                    data.channels = (value != "N/A" && !value.empty()) ? std::stoi(value) : 0;
                } catch (...) {
                    data.channels = 0;
                }
            } else if (key == "TAG:TITLE") {
                data.TITLE = value;
            } else if (key == "TAG:ARTIST") {
                data.ARTIST = value;
            } else if (key == "TAG:ALBUM") {
                data.ALBUM = value;
            } else if (key == "TAG:ALBUM_ARTIST") {
                data.ALBUM_ARTIST = value;
            } else if (key == "TAG:GENRE") {
                data.GENRE = value;
            } else if (key == "TAG:track") {
                try {
                    data.TRACK_NUMBER = std::stoi(value);
                } catch (...) {
                    data.TRACK_NUMBER = 0;
                }
            } else if (key == "TAG:disc") {
                try {
                    data.DISC_NUMBER = std::stoi(value);
                } catch (...) {
                    data.DISC_NUMBER = 0;
                }
            } else if (key == "TAG:YEAR") {
                try {
                    data.YEAR = std::stoi(value);
                } catch (...) {
                    data.YEAR = 0;
                }
            } else if (key == "TAG:DATE") {
                data.DATE = value;
            } else if (key == "TAG:COMMENT") {
                data.COMMENT = value;
            }
        }
        line_number++;
    }


    if (std::string(GlobalConf.OUTPUT_DIR).empty()) {
        std::cout
        << std::left << std::setw(20) << "Title:" << data.TITLE << "\n"
        << std::left << std::setw(20) << "Artist:" << data.ARTIST << "\n"
        << std::left << std::setw(20) << "Album:" << data.ALBUM << "\n"
        << std::left << std::setw(20) << "Album Artist:" << data.ALBUM_ARTIST << "\n"
        << std::left << std::setw(20) << "Genre:" << data.GENRE << "\n"
        << std::left << std::setw(20) << "Track Number:" << data.TRACK_NUMBER << "\n"
        << std::left << std::setw(20) << "Disc Number:" << data.DISC_NUMBER << "\n"
        << std::left << std::setw(20) << "Year:" << data.YEAR << "\n"
        << std::left << std::setw(20) << "Date:" << data.DATE << "\n"
        << std::left << std::setw(20) << "Comment:" << data.COMMENT << "\n"
        << std::left << std::setw(20) << "Codec Name:" << data.codec_name << "\n"
        << std::left << std::setw(20) << "Bitrate (kbps):" << data.bitrate_kbps << "\n"
        << std::left << std::setw(20) << "Sample Rate (Hz):" << data.sample_rate_hz << "\n"
        << std::left << std::setw(20) << "Channels:" << data.channels << "\n"
        << "----------------------------------------" << std::endl;
    } else if (std::filesystem::exists(GlobalConf.OUTPUT_DIR)) {
        // write/append to file
        std::ofstream outputfile;
        outputfile.open(GlobalConf.OUTPUT_PATH, std::ios::app);
        if (!outputfile.is_open()) {
            err("Failed to open output file for writing advanced metadata.");
        }
        outputfile
        << std::left << std::setw(20) << "Title:" << data.TITLE << "\n"
        << std::left << std::setw(20) << "Artist:" << data.ARTIST << "\n"
        << std::left << std::setw(20) << "Album:" << data.ALBUM << "\n"
        << std::left << std::setw(20) << "Album Artist:" << data.ALBUM_ARTIST << "\n"
        << std::left << std::setw(20) << "Genre:" << data.GENRE << "\n"
        << std::left << std::setw(20) << "Track Number:" << data.TRACK_NUMBER << "\n"
        << std::left << std::setw(20) << "Disc Number:" << data.DISC_NUMBER << "\n"
        << std::left << std::setw(20) << "Year:" << data.YEAR << "\n"
        << std::left << std::setw(20) << "Date:" << data.DATE << "\n"
        << std::left << std::setw(20) << "Comment:" << data.COMMENT << "\n"
        << std::left << std::setw(20) << "Codec Name:" << data.codec_name << "\n"
        << std::left << std::setw(20) << "Bitrate (kbps):" << data.bitrate_kbps << "\n"
        << std::left << std::setw(20) << "Sample Rate (Hz):" << data.sample_rate_hz << "\n"
        << std::left << std::setw(20) << "Channels:" << data.channels << "\n"
        << "----------------------------------------" << "\n";
        outputfile.close();
    }

    


    return data;
    }

    if (std::filesystem::exists(filepath) && std::filesystem::is_directory(filepath)) {

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
            auto info = ExtractAdvancedMeta(file);
        }

        AdvancedMetaData dummy;
        return dummy; // Return empty info when processing directory
    } else {
        err("Invalid audio file provided for ExtractAdvancedMeta.");
        AdvancedMetaData dummy;
        return dummy;
    }
} // extern AdvancedMetaData ExtractAdvancedMeta(const std::filesystem::path& filepath);

ReplayGainInfo MainFunctions::ExtractReplayGain(const std::filesystem::path& filepath) {
    if (ValidateAudioFile(filepath) && !std::filesystem::is_directory(filepath)) {
        ReplayGainInfo info;
        std::string command = "ffprobe -v error -select_streams a:0 -show_entries format_tags=replaygain_track_gain,replaygain_album_gain,replaygain_track_peak,replaygain_album_peak,title -of default=noprint_wrappers=1:nokey=1 \"" + filepath.string() + "\"";
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) err("ffprobe did not work, please ensure you have ffmpeg installed on your system.");
        std::array<char, 128> buffer;
        std::string result;
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }

        pclose(pipe);
        std::istringstream iss(result);
        std::string line;
        int line_number = 0;
        while (std::getline(iss, line)) {
            switch (line_number) {
                case 0:
                    try {
                        info.track_gain = (line.empty() || line == "N/A") ? 0.0f : std::stof(line);
                    } catch (...) {
                        info.track_gain = 0.0f;
                    }
                    break;
                case 1:
                    try {
                        info.album_gain = (line.empty() || line == "N/A") ? 0.0f : std::stof(line);
                    } catch (...) {
                        info.album_gain = 0.0f;
                    }
                    break;
                case 2:
                    try {
                        info.track_peak = (line.empty() || line == "N/A") ? 0.0f : std::stof(line);
                    } catch (...) {
                        info.track_peak = 0.0f;
                    }
                    break;
                case 3:
                    try {
                        info.album_peak = (line.empty() || line == "N/A") ? 0.0f : std::stof(line);
                    } catch (...) {
                        info.album_peak = 0.0f;
                    }
                    break;
                case 4:
                    info.TITLE = line;
                    break;
                default:
                    break;
            }
            line_number++;
        }

        if (std::string(GlobalConf.OUTPUT_DIR).empty()) {
            std::cout
            << std::left << std::setw(20) << "Title:" << info.TITLE << "\n"
            << std::left << std::setw(20) << "Track Gain (dB):" << info.track_gain << "\n"
            << std::left << std::setw(20) << "Album Gain (dB):" << info.album_gain << "\n"
            << std::left << std::setw(20) << "Track Peak:" << info.track_peak << "\n"
            << std::left << std::setw(20) << "Album Peak:" << info.album_peak << "\n"
            << "----------------------------------------" << std::endl;
        } else if (std::filesystem::exists(GlobalConf.OUTPUT_DIR)) {
            // write/append to file
            std::ofstream outputfile;
            outputfile.open(GlobalConf.OUTPUT_PATH, std::ios::app);
            if (!outputfile.is_open()) {
                err("Failed to open output file for writing replay gain info.");
            }
            outputfile
            << std::left << std::setw(20) << "Title:" << info.TITLE << "\n"
            << std::left << std::setw(20) << "Track Gain (dB):" << info.track_gain << "\n"
            << std::left << std::setw(20) << "Album Gain (dB):" << info.album_gain << "\n"
            << std::left << std::setw(20) << "Track Peak:" << info.track_peak << "\n"
            << std::left << std::setw(20) << "Album Peak:" << info.album_peak << "\n"
            << "----------------------------------------" << "\n";
            outputfile.close();
        }

        return info;
    }

    if (std::filesystem::exists(filepath) && std::filesystem::is_directory(filepath)) {

        std::vector<std::filesystem::path> audio_files;
        for (const auto& entry : std::filesystem::directory_iterator(filepath)) {
            if (ValidateAudioFile(entry.path())) {
                audio_files.push_back(entry.path());
            } else {
                warn("Skipping invalid audio file in directory: " + entry.path().string());
            }
        }

        std::sort(audio_files.begin(), audio_files.end());

        for (const auto& file : audio_files) {
            auto info = ExtractReplayGain(file);
        }

        ReplayGainInfo dummy;
        return dummy; 
    } else {
        err("Invalid audio file provided for ExtractReplayGain.");
        ReplayGainInfo dummy;
        return dummy;
    }
}   // extern ReplayGainInfo ExtractReplayGain(const std::filesystem::path& filepath);

// ADD MORE EXTRACTION FUNCTIONS AS NEEDED, FOLLOW THE SAME PATTERN AS ABOVE. (or try making it more efficient ya curious fuck)