#include "MainFunctions.hpp"
#include "Configuration.hpp"
extern GLOBAL_CONFIG GlobalConf;
#include <filesystem>
#include "Outputs.hpp"
using namespace Outputs;

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
} // Include all libraries just incase!! someone might want to use them later :3


// Place for calculation functions
// extern float CalculateReplayGainPerTrack(const std::filesystem::path& input_path);
// extern float CalculateReplayGainPerAlbum(const std::filesystem::path& input_path);





// extern void Convert(const std::filesystem::path& input_path, const std::string& target, const std::optional<std::filesystem::path>& output_path = std::nullopt);
void Convert(const std::filesystem::path& input_path, const std::string& target, const MainFunctions::AudioConversionTypes goalconversion,const std::optional<std::filesystem::path>& output_path) {
    // Conversion table
    //     TrueLLFLAC, // True lossless (-8) Compression FLAC (no compression is done here and is just tagged as flac)
    //     ArchivalFLAC, // Archival Flac (-5) Compression FLAC (likely compressed from -8 to -5)
    //     HighFidelityOPUS, // 512kbps OPUS (REDUNDANT but wtv)
    //     StandardOPUS, // 256kbps OPUS (this is considered standard for OPUS)
    //     StreamingOPUS, // 160kbps OPUS (good for home servers)
    //     PortableOPUS, // 128kbps OPUS (good for mobile devices)
    //     LowQualityOPUS, // 96kbps OPUS (good for podcasts)
    //     V0LegacyMP3, // V0 MP3 (Variable Bitrate, high quality for Cars/Old devices)
    //     StandardMP3, // 192kbps CBR MP3 (Standard quality MP3)
    //     LowQualityMP3, // 128kbps CBR MP3 (Balance between size and quality)
    //     VeryLowQualityMP3, // 96kbps CBR MP3 (Low quality MP3 for small sizes)
    //     OhMyGoshMP3, // 64kbps CBR MP3 (Ultra low quality, only for voice recordings)
    //     // Other standards can be added.. 
    //     ogg, // Other Formats without specific compression levels
    //     m4a, 
    //     wav,
    //     aac,
    //     wma,
    //     aiff,
    //     au,
    //     ra,
    //     a3ga,
    //     amr,
    //     awb,
    //     dss,
    //     dvf,
    //     m4b,
    //     m4p,
    //     mmf,
    //     mpc,
    //     msv,
    //     nmf,
    //     oga,
    //     raw,
    //     rf64,
    //     sln,
    //     tta,
    //     voc,
    //     vox,
    //     wv,
    //     webm,
    //     a8svx,
    //     cda,
}
