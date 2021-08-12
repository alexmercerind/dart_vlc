/*
 * dart_vlc: A media playback library for Dart & Flutter. Based on libVLC &
 * libVLC++.
 *
 * Hitesh Kumar Saini
 * https://github.com/alexmercerind
 * saini123hitesh@gmail.com; alexmercerind@gmail.com
 *
 * GNU Lesser General Public License v2.1
 */

#include <memory>

#include "../mediasource/playlist.hpp"
#include "../device.hpp"
#include "../equalizer.hpp"

class PlayerState {
 public:
  int32_t id_;
  int32_t index_ = 0;
  std::unique_ptr<Playlist> medias_ =
      std::make_unique<Playlist>(std::vector<std::shared_ptr<Media>>{});
  bool is_playing_ = false;
  bool is_valid_ = true;
  bool is_seekable_ = true;
  bool is_completed_ = false;
  int32_t position_ = 0;
  int32_t duration_ = 0;
  float volume_ = 1.0;
  float rate_ = 1.0;
  bool is_playlist_ = false;
  std::shared_ptr<Device> device_ = nullptr;
  bool is_started_ = false;

  void Reset() {
    medias_->medias_.clear();
    index_ = 0;
    is_playing_ = false;
    is_valid_ = true;
    is_seekable_ = true;
    is_completed_ = false;
    position_ = 0;
    duration_ = 0;
    is_started_ = false;
  }
};
