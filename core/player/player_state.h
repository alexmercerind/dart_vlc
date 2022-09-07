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

#ifndef PLAYER_PLAYER_STATE_H_
#define PLAYER_PLAYER_STATE_H_

#include <memory>

#include "devices/devices.h"
#include "equalizer/equalizer.h"
#include "media_source/playlist.h"

class PlayerState {
 public:
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

  void set_index(int32_t value) { index_ = value; };
  void set_medias(std::unique_ptr<Playlist> value) {
    medias_.reset(value.release());
  };
  void set_is_playing(bool value) { is_playing_ = value; };
  void set_is_valid(bool value) { is_valid_ = value; };
  void set_is_seekable(bool value) { is_seekable_ = value; };
  void set_is_completed(bool value) { is_completed_ = value; };
  void set_position(int32_t value) { position_ = value; };
  void set_duration(int32_t value) { duration_ = value; };
  void set_volume(float value) { volume_ = value; }
  void set_rate(float value) { rate_ = value; }
  void set_is_playlist(bool value) { is_playlist_ = value; };
  void set_is_started(bool value) { is_started_ = value; };

  void Reset();

 private:
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
};

#endif
