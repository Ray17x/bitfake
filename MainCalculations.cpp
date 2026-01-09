// extern void Convert(const std::filesystem::path& input_path, const std::string& target, const std::optional<std::filesystem::path>& output_path = std::nullopt);
// extern float CalculateReplayGainPerTrack(const std::filesystem::path& input_path);
// extern float CalculateReplayGainPerAlbum(const std::filesystem::path& input_path);

extern "C" {
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

// Git staging after finishing MainEXTRACTS.cpp.. please wait for this file to be finish which might be after 00:00 EST Jan 8th 2026.