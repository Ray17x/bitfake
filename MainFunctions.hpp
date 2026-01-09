#ifndef MAINFUNCTIONS_HPP
#define MAINFUNCTIONS_HPP

#include "Configuration.hpp"
#include "Outputs.hpp"
using namespace Outputs;
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <optional>

namespace MainFunctions {

    /*
        Inline is used here to seperate a function used across the rest of the functions to this one
        mainly for cleaniless but also looks cool.
    */

    enum class AudioConversionTypes {
        TrueLLFLAC, // True lossless (-8) Compression FLAC
        ArchivalFLAC, // Archival Flac (-5) Compression FLAC
        HighFidelityOPUS, // 512kbps OPUS
        StandardOPUS, // 256kbps OPUS
        StreamingOPUS, // 160kbps OPUS
        PortableOPUS, // 128kbps OPUS
        LowQualityOPUS, // 96kbps OPUS
        V0LegacyMP3, // V0 MP3
        StandardMP3, // 192kbps CBR MP3
        LowQualityMP3, // 128kbps CBR MP3
        VeryLowQualityMP3, // 96kbps CBR MP3
        OhMyGoshMP3, // 64kbps CBR MP3
        // Other standards can be added..
        ogg, // Other Formats without specific compression levels
        m4a,
        wav,
        aac,
        wma,
        aiff,
        au,
        ra,
        a3ga,
        amr,
        awb,
        dss,
        dvf,
        m4b,
        m4p,
        mmf,
        mpc,
        msv,
        nmf,
        oga,
        raw,
        rf64,
        sln,
        tta,
        voc,
        vox,
        wv,
        webm,
        a8svx,
        cda,
    };

    // inline const std::vector<std::string_view> AudioConversionTypeNames = {
    //     "TrueLLFLAC", // No compression is done here, file is tagged or operations are rooted from files like this.
    //     "ArchivalFLAC", // Lowest compression FLAC for archival purposes (Slight space saver on bigger libraries).
    //     "HighFidelityOPUS", // 512kbps OPUS (likely redundant but eh)
    //     "StandardOPUS", // 256kbps OPUS (yes! this is considered standard for OPUS)
    //     "StreamingOPUS", // 160kpbs OPUS (Good for home servers!)
    //     "PortableOPUS", // 128kbps OPUS (Good for mobile devices)
    //     "LowQualityOPUS", // 96kbps OPUS (Low quality but still acceptable for podcasts)
    //     "V0LegacyMP3", // V0 MP3 (Variable Bitrate, high quality for Cars/Old devices)
    //     "StandardMP3", // 192kbps CBR MP3 (Standard quality MP3)
    //     "LowQualityMP3", // 128kbps CBR MP3 (Good balance between size and quality)
    //     "VeryLowQualityMP3", // 96kbps CBR MP3 (Low quality MP3 for small sizes)
    //     "OhMyGoshMP3", // 64kbps CBR MP3 (Ultra low quality, only for voice recordings)
    //     // Other standards can be added..
    //     "ogg", // Other Formats without specific compression levels
    //     "m4a",
    //     "wav",
    //     "aac",
    //     "wma",
    //     "aiff",
    //     "au",
    //     "ra",
    //     "3ga",
    //     "amr",
    //     "awb",
    //     "dss",
    //     "dvf",
    //     "m4b",
    //     "m4p",
    //     "mmf",
    //     "mpc",
    //     "msv",
    //     "nmf",
    //     "oga",
    //     "raw",
    //     "rf64",
    //     "sln",
    //     "tta",
    //     "voc",
    //     "vox",
    //     "wv",
    //     "webm",
    //     "8svx",
    //     "cda",
    // };

    inline bool ValidateAudioFile(const std::filesystem::path& path) {
       // Check for audio file extensions
        const std::vector<std::string_view> audio_extensions = {
                "mp3",
                "ogg",
                "m4a",
                "wav",
                "flac",
                "aac",
                "wma",
                "opus",
                "aiff",
                "au",
                "ra",
                "3ga",
                "amr",
                "awb",
                "dss",
                "dvf",
                "m4b",
                "m4p",
                "mmf",
                "mpc",
                "msv",
                "nmf",
                "oga",
                "raw",
                "rf64",
                "sln",
                "tta",
                "voc",
                "vox",
                "wv",
                "webm",
                "8svx",
                "cda",
        }; // I just added a bunch of commons ones, add them at the end as you feel fit if you need your files to work
        // nvm thanks to kaizen from all things linux for the list of extensions
        
        if (!std::filesystem::exists(path)) { warn("File doesn't exist."); return false; }
        std::string ext = path.extension().string();
        // Remove the leading dot from the extension
        if (!ext.empty() && ext[0] == '.') {
            ext = ext.substr(1);
        }
        // Convert to lowercase for case-insensitive comparison
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (std::find(audio_extensions.begin(), audio_extensions.end(), ext) != audio_extensions.end()) {
            // log("Valid audio file: " + path.string());
            return true;
        } else {
            warn("Unsupported audio file format: " + path.string());
            return false;
        }
    }

    struct BasicCodecInfo {
        std::string song_title;
        std::string file_path;
        std::string codec_name;
        int bitrate_kbps;
        int sample_rate_hz;
        int channels; // likely returns 0 for unknown reasons.. help me figure this out
    };

    struct AdvancedMetaData {
        // Basic Info [IMPORTANT!!]
        std::string TITLE;
        std::string ARTIST;
        std::string ALBUM;
        std::string ALBUM_ARTIST;
        std::string GENRE;

        // Track Info
        int TRACK_NUMBER;
        int TOTAL_TRACKS;
        int DISC_NUMBER;
        int TOTAL_DISCS;

        // Release info

        int YEAR;
        std::string DATE;

        // Comments 
        // Will be using comments to tag the fact that it has been confirmed to be a real lossless audio file.
        std::string COMMENT;

        // codec info

        std::string codec_name;
        int bitrate_kbps;
        int sample_rate_hz;
        int channels; // likely returns 0 for unknown reasons..S
    };

    struct ReplayGainInfo {
        std::string TITLE;
        std::string file_path;
        float track_gain;
        float album_gain;
        float track_peak;
        float album_peak;

        // Likely the most important info for audio 
    };

    // Data extracting

    extern BasicCodecInfo ExtractBasicCodec(const std::filesystem::path& filepath);
    extern AdvancedMetaData ExtractAdvancedMeta(const std::filesystem::path& filepath);
    extern ReplayGainInfo ExtractReplayGain(const std::filesystem::path& filepath);

    // Processing functions
    
    extern void Convert(const std::filesystem::path& input_path, const std::string& target, const AudioConversionTypes goalconversion, const std::optional<std::filesystem::path>& output_path = std::nullopt);
    extern float CalculateReplayGainPerTrack(const std::filesystem::path& input_path);
    extern float CalculateReplayGainPerAlbum(const std::filesystem::path& input_path);
    //// More functions can be added here as needed.

}

#endif // MAINFUCTIONS_HPP