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

#include "internal/internal.h"

class PlayerGetters : public PlayerInternal {
 public:
  int32_t video_width() const { return video_width_; }

  int32_t video_height() const { return video_height_; }

  PlayerState* state() const { return state_.get(); }

  int32_t duration() {
    return static_cast<int32_t>(vlc_media_player_.length());
  }

  int32_t position() {
    return static_cast<int32_t>(vlc_media_player_.length() *
                                vlc_media_player_.position());
  }

  float volume() { return vlc_media_player_.volume() / 100.0f; }

  float rate() { return vlc_media_player_.rate(); }

  bool is_playing() { return vlc_media_player_.isPlaying(); }

  bool is_paused() { return !vlc_media_player_.isPlaying(); }
};
