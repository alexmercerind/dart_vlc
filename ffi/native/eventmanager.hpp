/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC &
 * libVLC++.
 *
 * Hitesh Kumar Saini & contributors.
 * https://github.com/alexmercerind
 * alexmercerind@gmail.com
 *
 * GNU Lesser General Public License v2.1
 */

#include "../../dartvlc/main.cpp"
#include "callbackmanager.hpp"

#ifdef __cplusplus
extern "C" {
#endif

inline void OnPlayPauseStop(int32_t id, PlayerState* state) {
  std::vector<std::string> event{std::to_string(id), "playbackEvent",
                                 std::to_string(state->is_playing()),
                                 std::to_string(state->is_seekable())};

  CallbackStringArray(event);
}

inline void OnPosition(int32_t id, PlayerState* state) {
  std::vector<std::string> event{
      std::to_string(id), "positionEvent", std::to_string(state->index()),
      std::to_string(state->position()), std::to_string(state->duration())};
  CallbackStringArray(event);
}

inline void OnComplete(int32_t id, PlayerState* state) {
  std::vector<std::string> event{
      std::to_string(id), "completeEvent",
      std::to_string(state->is_completed()),
  };
  CallbackStringArray(event);
}

inline void OnVolume(int32_t id, PlayerState* state) {
  std::vector<std::string> event{std::to_string(id), "volumeEvent",
                                 std::to_string(state->volume())};
  CallbackStringArray(event);
}

inline void OnRate(int32_t id, PlayerState* state) {
  std::vector<std::string> event{std::to_string(id), "rateEvent",
                                 std::to_string(state->rate())};
  CallbackStringArray(event);
}

inline void OnOpen(int32_t id, PlayerState* state) {
  const auto& media_items = state->medias()->medias();
  std::vector<std::string> event;
  event.reserve(4 + media_items.size() * 2);
  event.emplace_back(std::to_string(id));
  event.emplace_back("openEvent");
  event.emplace_back(std::to_string(state->index()));
  event.emplace_back(std::to_string(state->is_playlist()));
  for (const auto& media : media_items) {
    event.emplace_back(media->media_type());
    event.emplace_back(media->resource());
  }
  CallbackStringArray(event);
}

inline void OnVideoDimension(int32_t id, int32_t video_width,
                             int32_t video_height) {
  std::vector<std::string> event{std::to_string(id), "videoDimensionEvent",
                                 std::to_string(video_width),
                                 std::to_string(video_height)};
  CallbackStringArray(event);
}

inline void OnVideo(int32_t id, int size, PlayerState* state, uint8_t* frame) {
  CallbackFrame(id, size, frame);
}

#ifdef __cplusplus
}
#endif
