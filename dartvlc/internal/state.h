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

#include "mediasource/playlist.h"
#include "device.h"
#include "equalizer.h"

class PlayerState {
 public:
  void Reset() {
    medias_->medias().clear();
    index_ = 0;
    is_playing_ = false;
    is_valid_ = true;
    is_seekable_ = true;
    is_completed_ = false;
    position_ = 0;
    duration_ = 0;
    is_started_ = false;
  }

  int32_t index() const { return index_; };
  Playlist* medias() const { return medias_.get(); };
  bool is_playing() const { return is_playing_; };
  bool is_valid() const { return is_valid_; };
  bool is_seekable() const { return is_seekable_; };
  bool is_completed() const { return is_completed_; };
  int32_t position() const { return position_; };
  int32_t duration() const { return duration_; };
  float volume() const { return volume_; }
  float rate() const { return rate_; }
  bool is_playlist() const { return is_playlist_; };
  bool is_started() const { return is_started_; };

 protected:
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
  bool is_started_ = false;

  friend class PlayerSetters;
  friend class PlayerEvents;
};
