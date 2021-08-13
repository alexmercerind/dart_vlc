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

#include "../../dartvlc/main.hpp"
#include "callbackmanager.hpp"

#ifdef __cplusplus
extern "C" {
#endif

inline void OnPlayPauseStop(int32_t id, PlayerState* state) {
  std::vector<std::string> event{};
  event.emplace_back(std::to_string(id));
  event.emplace_back("playbackEvent");
  event.emplace_back(std::to_string(state->is_playing()));
  event.emplace_back(std::to_string(state->is_seekable()));
  size_t size = event.size();
  char** event_data = new char*[size];
  for (int index = 0; index < size; index++)
    event_data[index] = const_cast<char*>(event[index].c_str());
  CallbackStringArray(static_cast<int>(size), event_data);

  delete[] event_data;
}

inline void OnPosition(int32_t id, PlayerState* state) {
  std::vector<std::string> event{};
  event.emplace_back(std::to_string(id));
  event.emplace_back("positionEvent");
  event.emplace_back(std::to_string(state->index()));
  event.emplace_back(std::to_string(state->position()));
  event.emplace_back(std::to_string(state->duration()));
  size_t size = event.size();
  char** event_data = new char*[size];
  for (int index = 0; index < size; index++)
    event_data[index] = const_cast<char*>(event[index].c_str());
  CallbackStringArray(static_cast<int>(size), event_data);

  delete[] event_data;
}

inline void OnComplete(int32_t id, PlayerState* state) {
  std::vector<std::string> event{};
  event.emplace_back(std::to_string(id));
  event.emplace_back("completeEvent");
  event.emplace_back(std::to_string(state->is_completed()));
  size_t size = event.size();
  char** event_data = new char*[size];
  for (int index = 0; index < size; index++)
    event_data[index] = const_cast<char*>(event[index].c_str());
  CallbackStringArray(static_cast<int>(size), event_data);

  delete[] event_data;
}

inline void OnVolume(int32_t id, PlayerState* state) {
  std::vector<std::string> event{};
  event.emplace_back(std::to_string(id));
  event.emplace_back("volumeEvent");
  event.emplace_back(std::to_string(state->volume()));
  size_t size = event.size();
  char** event_data = new char*[size];
  for (int index = 0; index < size; index++)
    event_data[index] = const_cast<char*>(event[index].c_str());
  CallbackStringArray(static_cast<int>(size), event_data);

  delete[] event_data;
}

inline void OnRate(int32_t id, PlayerState* state) {
  std::vector<std::string> event{};
  event.emplace_back(std::to_string(id));
  event.emplace_back("rateEvent");
  event.emplace_back(std::to_string(state->rate()));
  size_t size = event.size();
  char** event_data = new char*[size];
  for (int index = 0; index < size; index++)
    event_data[index] = const_cast<char*>(event[index].c_str());
  CallbackStringArray(static_cast<int>(size), event_data);

  delete[] event_data;
}

inline void OnOpen(int32_t id, PlayerState* state) {
  std::vector<std::string> event{};
  event.emplace_back(std::to_string(id));
  event.emplace_back("openEvent");
  event.emplace_back(std::to_string(state->index()));
  event.emplace_back(std::to_string(state->is_playlist()));
  for (std::shared_ptr<Media> media : state->medias()->medias()) {
    event.emplace_back(media->media_type());
    event.emplace_back(media->resource());
  }
  size_t size = event.size();
  char** event_data = new char*[size];
  for (int index = 0; index < size; index++)
    event_data[index] = const_cast<char*>(event[index].c_str());
  CallbackStringArray(static_cast<int>(size), event_data);

  delete[] event_data;
}

inline void OnVideo(int size, int32_t id, PlayerState* state, uint8_t* frame) {
  CallbackFrame(size, id, frame);
}

#ifdef __cplusplus
}
#endif
