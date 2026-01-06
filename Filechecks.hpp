#ifndef FILECHECKS_HPP
#define FILECHECKS_HPP

#include <filesystem>
#include "Output.hpp"
#include <unordered_set>
#include <string>
#include <algorithm>
using namespace Output;

namespace FileChecks {


    // Checks overall if the file is an AUDIO file too.
    static const std::unordered_set<std::string> audio_extensions = {
        ".mp3", ".wav", ".flac", ".ogg", ".m4a", ".aac", 
        ".wma", ".aiff", ".opus", ".mid", ".midi"
    };
    inline bool ValidateAudioFileExistance(const std::filesystem::path& path) {
        if (!path.has_extension()) return false;
        std::string ext = path.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        return audio_extensions.find(ext) != audio_extensions.end() && std::filesystem::exists(path);
    }
    inline bool IsDir(const std::filesystem::path& path) {
        if (!ValidateAudioFileExistance(path)) {
            return false;
        } else if (!std::filesystem::is_directory(path)) {
            return false;
        }
        return true;
    }

    struct AudioCodecInfo {
        std::string codec_name;
        int bitrate;
        int sample_rate;
        int channels;
        bool valid = false;
    };

    struct AudioMetadata {
        std::string ALBUM;
        std::string album_artist;
        std::string ARTIST;
        std::string DATE; // String so it can hold YEAR, MONTH, and DAY if needed.
        int disc;
        int DISCTOTAL;
        double REPLAYGAIN_ALBUM_GAIN;
        double REPLAYGAIN_ALBUM_PEAK;
        double REPLAYGAIN_TRACK_GAIN;
        double REPLAYGAIN_TRACK_PEAK;
        std::string TITLE;
        int track;
        int TRACKTOTAL;
        std::string GENRE;
        std::string COMMENT;
    };

    // Defined function in filechecks.cpp
    extern AudioCodecInfo ExtractAudioCodecInfo(const std::filesystem::path& file);
    extern AudioMetadata ExtractAudioMetadata(const std::filesystem::path& file);
    extern void SpecturalAnalysis(const std::filesystem::path& path);
    extern void BatchConvert(const std::filesystem::path& dir);
    extern void BatchQualityVerify(const std::filesystem::path& dir);
    extern void SingleFileConvert(const std::filesystem::path& file);
    extern void SingleFileQualityVerify(const std::filesystem::path& file);
    extern void LogToFile(const std::string& log_msg, const std::filesystem::path& log_file);
    extern void GenerateSpectogram(const std::filesystem::path& file, const std::filesystem::path& output_image);
}

#endif // FILECHECKS_HPP