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

#ifdef _WIN32
#include <vlcpp/vlc.hpp>
#else
#include "vlcpp/vlc.hpp"
#endif

class PlayerInternal {
 protected:
  VLC::Instance vlc_instance_;
  VLC::MediaPlayer vlc_media_player_;
  VLC::MediaListPlayer vlc_media_list_player_;
  VLC::MediaList vlc_media_list_;
  std::unique_ptr<uint8_t> video_frame_buffer_;
  int32_t video_width_ = 0;
  int32_t video_height_ = 0;
  bool is_playlist_modified_ = false;
};
