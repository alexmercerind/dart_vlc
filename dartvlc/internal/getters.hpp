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

#include "internal.hpp"
#include "state.hpp"

class PlayerGetters : protected PlayerInternal {
 public:
  std::unique_ptr<PlayerState> state_;

  int32_t Duration() {
    return static_cast<int32_t>(vlc_media_player_.length());
  }

  int32_t Position() {
    return static_cast<int32_t>(vlc_media_player_.length() *
                                vlc_media_player_.position());
  }

  float Volume() {
    float volume = vlc_media_player_.volume() / 100.0f;
    return volume;
  }

  float Rate() { return vlc_media_player_.rate(); }

  bool IsPlaying() { return vlc_media_player_.isPlaying(); }

  bool IsPaused() { return !vlc_media_player_.isPlaying(); }
};
