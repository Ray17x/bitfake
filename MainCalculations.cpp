#include "MainFunctions.hpp"
#include "Configuration.hpp"
extern GLOBAL_CONFIG GlobalConf;
#include <filesystem>
#include "Outputs.hpp"
using namespace Outputs;
#include <string>
#include <cstdio>
#include <array>
#include <sstream>

// extern "C" {
// #include <libavcodec/avcodec.h>
// #include <libavformat/avformat.h>
// #include <libavutil/imgutils.h>
// #include <libavutil/avutil.h>
// #include <libswscale/swscale.h>
// } // Include all libraries just incase!! someone might want to use them later :3


// Place for calculation functions
// extern float CalculateReplayGainPerTrack(const std::filesystem::path& input_path);
// extern float CalculateReplayGainPerAlbum(const std::filesystem::path& input_path);





// extern void Convert(const std::filesystem::path& input_path, const std::string& target, const std::optional<std::filesystem::path>& output_path = std::nullopt);

namespace {
    // Helper function to escape shell special characters in file paths
    // Uses single quotes which is safer than double quotes for shell arguments
    std::string EscapeShellPath(const std::string& path) {
        // In single quotes, only the single quote itself needs escaping
        // We escape it by ending the quoted string, adding an escaped single quote, and starting a new quoted string
        std::string escaped = "'";
        for (char c : path) {
            if (c == '\'') {
                escaped += "'\\''";  // End quote, add escaped single quote, start new quote
            } else {
                escaped += c;
            }
        }
        escaped += "'";
        return escaped;
    }
    
    // Helper function to build ffmpeg command based on conversion type
    std::string BuildFFmpegCommand(const std::filesystem::path& input, const std::filesystem::path& output, const MainFunctions::AudioConversionTypes type) {
        std::string cmd = "ffmpeg -i " + EscapeShellPath(input.string()) + " ";
        
        switch (type) {
            // FLAC conversions
            case MainFunctions::AudioConversionTypes::TrueLLFLAC:
                cmd += "-c:a flac -compression_level 8 ";
                break;
            case MainFunctions::AudioConversionTypes::ArchivalFLAC:
                cmd += "-c:a flac -compression_level 5 ";
                break;
            
            // OPUS conversions
            case MainFunctions::AudioConversionTypes::HighFidelityOPUS:
                cmd += "-c:a libopus -b:a 512k ";
                break;
            case MainFunctions::AudioConversionTypes::StandardOPUS:
                cmd += "-c:a libopus -b:a 256k ";
                break;
            case MainFunctions::AudioConversionTypes::StreamingOPUS:
                cmd += "-c:a libopus -b:a 160k ";
                break;
            case MainFunctions::AudioConversionTypes::PortableOPUS:
                cmd += "-c:a libopus -b:a 128k ";
                break;
            case MainFunctions::AudioConversionTypes::LowQualityOPUS:
                cmd += "-c:a libopus -b:a 96k ";
                break;
            
            // MP3 conversions
            case MainFunctions::AudioConversionTypes::V0LegacyMP3:
                cmd += "-c:a libmp3lame -q:a 0 ";
                break;
            case MainFunctions::AudioConversionTypes::StandardMP3:
                cmd += "-c:a libmp3lame -b:a 192k ";
                break;
            case MainFunctions::AudioConversionTypes::LowQualityMP3:
                cmd += "-c:a libmp3lame -b:a 128k ";
                break;
            case MainFunctions::AudioConversionTypes::VeryLowQualityMP3:
                cmd += "-c:a libmp3lame -b:a 96k ";
                break;
            case MainFunctions::AudioConversionTypes::OhMyGoshMP3:
                cmd += "-c:a libmp3lame -b:a 64k ";
                break;
            
            // Other formats - basic conversion
            case MainFunctions::AudioConversionTypes::ogg:
                cmd += "-c:a libvorbis ";
                break;
            case MainFunctions::AudioConversionTypes::m4a:
                cmd += "-c:a aac ";
                break;
            case MainFunctions::AudioConversionTypes::wav:
                cmd += "-c:a pcm_s16le ";
                break;
            case MainFunctions::AudioConversionTypes::aac:
                cmd += "-c:a aac ";
                break;
            case MainFunctions::AudioConversionTypes::wma:
                cmd += "-c:a wmav2 ";
                break;
            case MainFunctions::AudioConversionTypes::aiff:
                cmd += "-c:a pcm_s16be ";
                break;
            default:
                // Use FFmpeg's automatic codec detection for other formats
                break;
        }
        
        cmd += "-y " + EscapeShellPath(output.string()) + " 2>&1";
        return cmd;
    }

    // Helper function to get file extension for conversion type
    std::string GetExtensionForType(const MainFunctions::AudioConversionTypes type) {
        switch (type) {
            case MainFunctions::AudioConversionTypes::TrueLLFLAC:
            case MainFunctions::AudioConversionTypes::ArchivalFLAC:
                return ".flac";
            case MainFunctions::AudioConversionTypes::HighFidelityOPUS:
            case MainFunctions::AudioConversionTypes::StandardOPUS:
            case MainFunctions::AudioConversionTypes::StreamingOPUS:
            case MainFunctions::AudioConversionTypes::PortableOPUS:
            case MainFunctions::AudioConversionTypes::LowQualityOPUS:
                return ".opus";
            case MainFunctions::AudioConversionTypes::V0LegacyMP3:
            case MainFunctions::AudioConversionTypes::StandardMP3:
            case MainFunctions::AudioConversionTypes::LowQualityMP3:
            case MainFunctions::AudioConversionTypes::VeryLowQualityMP3:
            case MainFunctions::AudioConversionTypes::OhMyGoshMP3:
                return ".mp3";
            case MainFunctions::AudioConversionTypes::ogg:
            case MainFunctions::AudioConversionTypes::oga:
                return ".ogg";
            case MainFunctions::AudioConversionTypes::m4a:
            case MainFunctions::AudioConversionTypes::m4b:
            case MainFunctions::AudioConversionTypes::m4p:
                return ".m4a";
            case MainFunctions::AudioConversionTypes::wav:
                return ".wav";
            case MainFunctions::AudioConversionTypes::aac:
                return ".aac";
            case MainFunctions::AudioConversionTypes::wma:
                return ".wma";
            case MainFunctions::AudioConversionTypes::aiff:
                return ".aiff";
            case MainFunctions::AudioConversionTypes::au:
                return ".au";
            case MainFunctions::AudioConversionTypes::ra:
                return ".ra";
            case MainFunctions::AudioConversionTypes::a3ga:
                return ".a3ga";
            case MainFunctions::AudioConversionTypes::amr:
                return ".amr";
            case MainFunctions::AudioConversionTypes::awb:
                return ".awb";
            case MainFunctions::AudioConversionTypes::dss:
                return ".dss";
            case MainFunctions::AudioConversionTypes::dvf:
                return ".dvf";
            case MainFunctions::AudioConversionTypes::mmf:
                return ".mmf";
            case MainFunctions::AudioConversionTypes::mpc:
                return ".mpc";
            case MainFunctions::AudioConversionTypes::msv:
                return ".msv";
            case MainFunctions::AudioConversionTypes::nmf:
                return ".nmf";
            case MainFunctions::AudioConversionTypes::raw:
                return ".raw";
            case MainFunctions::AudioConversionTypes::rf64:
                return ".rf64";
            case MainFunctions::AudioConversionTypes::sln:
                return ".sln";
            case MainFunctions::AudioConversionTypes::tta:
                return ".tta";
            case MainFunctions::AudioConversionTypes::voc:
                return ".voc";
            case MainFunctions::AudioConversionTypes::vox:
                return ".vox";
            case MainFunctions::AudioConversionTypes::wv:
                return ".wv";
            case MainFunctions::AudioConversionTypes::webm:
                return ".webm";
            case MainFunctions::AudioConversionTypes::a8svx:
                return ".a8svx";
            case MainFunctions::AudioConversionTypes::cda:
                return ".cda";
            default:
                return ".mp3"; // Default fallback
        }
    }
}

void MainFunctions::Convert(const std::filesystem::path& input_path, const std::string& target, const MainFunctions::AudioConversionTypes goalconversion, const std::optional<std::filesystem::path>& output_path) {
    // Check if input exists
    if (!std::filesystem::exists(input_path)) {
        err("Input path does not exist: " + input_path.string());
        return;
    }
    
    // If input is a directory, recursively process all audio files
    if (std::filesystem::is_directory(input_path)) {
        log("Processing directory: " + input_path.string());
        
        // Determine output directory
        std::filesystem::path out_dir;
        if (output_path.has_value()) {
            out_dir = output_path.value();
            // Create output directory if it doesn't exist
            if (!std::filesystem::exists(out_dir)) {
                std::filesystem::create_directories(out_dir);
            }
        } else {
            // Default: create subdirectory with target name
            out_dir = input_path / target;
            if (!std::filesystem::exists(out_dir)) {
                std::filesystem::create_directories(out_dir);
            }
        }
        
        // Recursively iterate through directory
        for (const auto& entry : std::filesystem::recursive_directory_iterator(input_path)) {
            if (entry.is_regular_file() && MainFunctions::ValidateAudioFile(entry.path())) {
                // Calculate relative path for maintaining directory structure
                std::filesystem::path relative = std::filesystem::relative(entry.path(), input_path);
                std::filesystem::path output_file = out_dir / relative;
                
                // Change extension based on conversion type
                output_file.replace_extension(GetExtensionForType(goalconversion));
                
                // Create parent directory if needed
                std::filesystem::path output_parent = output_file.parent_path();
                if (!std::filesystem::exists(output_parent)) {
                    std::filesystem::create_directories(output_parent);
                }
                
                // Convert the file
                MainFunctions::Convert(entry.path(), target, goalconversion, std::make_optional(output_file));
            }
        }
        
        log("Directory processing complete.");
        return;
    }
    
    // If input is a file, validate it's an audio file
    if (!MainFunctions::ValidateAudioFile(input_path)) {
        warn("Skipping non-audio file: " + input_path.string());
        return;
    }
    
    // Determine output file path
    std::filesystem::path output_file;
    if (output_path.has_value()) {
        output_file = output_path.value();
    } else {
        // Default: same directory, add target suffix and new extension
        std::filesystem::path parent = input_path.parent_path();
        std::string stem = input_path.stem().string();
        std::string new_extension = GetExtensionForType(goalconversion);
        output_file = parent / (stem + "_" + target + new_extension);
    }
    
    log("Converting: " + input_path.filename().string() + " -> " + output_file.filename().string());
    
    // Build and execute ffmpeg command
    std::string command = BuildFFmpegCommand(input_path, output_file, goalconversion);
    
    if (GlobalConf.verbose) {
        log("Executing: " + command);
    }
    
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        err("Failed to execute ffmpeg command for: " + input_path.filename().string());
        return;
    }
    
    // Read output
    std::array<char, 256> buffer;
    std::ostringstream result_stream;
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result_stream << buffer.data();
        if (GlobalConf.verbose) {
            std::cout << buffer.data();
        }
    }
    
    int exit_code = pclose(pipe);
    
    if (exit_code == 0) {
        log("Successfully converted: " + output_file.string());
    } else {
        warn("Conversion may have encountered issues. Exit code: " + std::to_string(exit_code));
        if (!GlobalConf.verbose) {
            std::string result = result_stream.str();
            if (!result.empty()) {
                std::cout << "FFmpeg output:\n" << result << std::endl;
            }
        }
    }
}
