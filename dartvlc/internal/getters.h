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
