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

#include <optional>
#include <vlcpp/vlc.hpp>

#include "internal/state.h"

class PlayerInternal {
 protected:
  VLC::Instance vlc_instance_;
  VLC::MediaPlayer vlc_media_player_;
  VLC::MediaListPlayer vlc_media_list_player_;
  VLC::MediaList vlc_media_list_;
  std::unique_ptr<PlayerState> state_ = nullptr;
  std::unique_ptr<uint8_t> video_frame_buffer_ = nullptr;
  int32_t video_width_ = 0;
  int32_t video_height_ = 0;
  std::optional<int32_t> preferred_video_width_ = std::nullopt;
  std::optional<int32_t> preferred_video_height_ = std::nullopt;
  bool is_playlist_modified_ = false;
};
