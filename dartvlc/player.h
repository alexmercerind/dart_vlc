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

#ifndef PLAYER_H_
#define PLAYER_H_

#include <memory>
#include <mutex>

#include "internal/setters.h"

static auto TO_CHARARRAY(const std::vector<std::string>& vector) {
  size_t size = vector.size();
  auto array = std::unique_ptr<const char*[]>(new const char*[size]);

  for (auto i = 0; i < size; i++) {
    array[i] = vector[i].c_str();
  }

  return array;
}

class Player : public PlayerSetters {
 public:
  Player(const std::vector<std::string>& cmd_arguments) {
    if (cmd_arguments.empty()) {
      vlc_instance_ = VLC::Instance(0, nullptr);
    } else {
      auto args = TO_CHARARRAY(cmd_arguments);
      vlc_instance_ =
          VLC::Instance(static_cast<int32_t>(cmd_arguments.size()), args.get());
    }
    vlc_media_player_ = VLC::MediaPlayer(vlc_instance_);
    vlc_media_list_player_ = VLC::MediaListPlayer(vlc_instance_);
    vlc_media_list_ = VLC::MediaList(vlc_instance_);
    vlc_media_list_player_.setMediaPlayer(vlc_media_player_);
    state_ = std::make_unique<PlayerState>();
    vlc_media_player_.setVolume(100);
  }

  ~Player() { vlc_media_player_.stop(); }
};

class Players {
 public:
  Player* Get(int32_t id, std::vector<std::string> cmd_arguments = {}) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto [it, added] = players_.try_emplace(id, nullptr);
    if (added) {
      it->second = std::make_unique<Player>(cmd_arguments);
    }
    return it->second.get();
  }

  void Dispose(int32_t id) {
    std::lock_guard<std::mutex> lock(mutex_);
    players_.erase(id);
  }

 private:
  std::mutex mutex_;
  std::map<int32_t, std::unique_ptr<Player>> players_;
};

extern std::unique_ptr<Players> g_players;

#endif
