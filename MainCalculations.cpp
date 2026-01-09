// extern void Convert(const std::filesystem::path& input_path, const std::string& target, const std::optional<std::filesystem::path>& output_path = std::nullopt);
// extern float CalculateReplayGainPerTrack(const std::filesystem::path& input_path);
// extern float CalculateReplayGainPerAlbum(const std::filesystem::path& input_path);

extern "C" {
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}