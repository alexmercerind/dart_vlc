// This file is a part of dart_vlc (https://github.com/alexmercerind/dart_vlc)
//
// Copyright (C) 2021-2022 Hitesh Kumar Saini <saini123hitesh@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

// https://github.com/alexmercerind/dart_vlc/pull/137
#define BUFFER_SIZE 67108864

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
  std::unique_ptr<uint8_t[]> video_frame_buffer_ =
      std::make_unique<uint8_t[]>(BUFFER_SIZE);
  int32_t video_width_ = -1;
  int32_t video_height_ = -1;
  std::optional<int32_t> preferred_video_width_ = std::nullopt;
  std::optional<int32_t> preferred_video_height_ = std::nullopt;
  bool is_playlist_modified_ = false;
};
